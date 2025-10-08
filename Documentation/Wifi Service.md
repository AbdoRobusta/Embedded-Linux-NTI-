# WiFi Static IP package for Raspberry Pi (systemd + Yocto)

## Summary
This package contains a small systemd unit, a helper script, and a wpa_supplicant configuration to set up WiFi with a static IP on `wlan0` at boot. It also includes a Yocto recipe to install these files into an image and enable the systemd service automatically.

**Files included in the package:**
- `wpa_supplicant.conf` — WiFi network definition (SSID + PSK).
- `wifi-static.sh` — Script that brings up `wlan0`, runs `wpa_supplicant`, assigns a static IP, gateway and DNS.
- `wifi-static.service` — systemd unit (Type=oneshot) that runs the script at boot.
- `wifi-static_0.1.bb` — Yocto recipe to install files to the target image and enable the service.

---

## Why this exists
In minimal or custom embedded images (e.g. Yocto) you may not have a network manager (dhcpcd, NetworkManager, connman) configured. This package provides a simple, deterministic way to bring WiFi up with a fixed IP and a stable `wlan0` interface immediately after boot, which can be useful for headless devices, test rigs, or controlled networks.

---

## File-by-file explanation

### `wifi-static.service`
- Location (in the image): `/lib/systemd/system/wifi-static.service`
- Purpose: Run `/usr/bin/wifi-static.sh` once during boot (oneshot). `RemainAfterExit=yes` keeps the unit in an "active" state after the script finishes so other services can depend on it; the unit is enabled for `multi-user.target`.
- Important change vs. naive setups: It uses `After=network-online.target` and `Wants=network-online.target` to better ensure that network interfaces are up before the unit runs. Use `multi-user.target` in the `[Install]` stanza (not a user target).

### `wifi-static.sh`
- Location (in the image): `/usr/bin/wifi-static.sh`
- Purpose: Ensure `wlan0` is up, run `wpa_supplicant` (if not running), assign a static IP using `ip` commands, set the default route, and write DNS to `/etc/resolv.conf`.
- Robustness improvements included:
  - Uses `ip` instead of legacy `ifconfig`/`route` to improve portability.
  - Unblocks rfkill and confirms that `wlan0` exists before proceeding.
  - Disables power-saving on the interface to avoid connectivity drops.
  - Flushes old addresses before adding the static IP.
  - Writes a proper nameserver (not a `.0` network address).
- Notes: this script assumes there is no other network manager actively reconfiguring `wlan0`. If your distro uses `dhcpcd` or NetworkManager, prefer configuring static IPs via those tools to avoid conflicts.

### `wpa_supplicant.conf`
- Purpose: Holds WiFi network credentials and country code. Permissions must be restricted (0600) because it contains the PSK.
- Tip: `ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev` allows `wpa_cli` and other tooling to control the interface.

### `wifi-static_0.1.bb` (Yocto recipe)
- Purpose: Installs the three files to their target locations and registers the systemd service with `SYSTEMD_SERVICE_${PN}` and `SYSTEMD_AUTO_ENABLE_${PN}`.
- Notes and corrections applied:
  - Use `FILES_${PN}` to include installed files.
  - Use `SYSTEMD_SERVICE_${PN}` and `SYSTEMD_AUTO_ENABLE_${PN}` to inform `systemd` class how to enable the unit.
  - `inherit allarch systemd` is used because the package is architecture independent and needs systemd handling.

---

## How to test on a running device (manual install)

1. Copy files to the device paths (example assumes root access):
```bash
# copy files to the Pi
sudo cp wpa_supplicant.conf /etc/wpa_supplicant/wpa_supplicant.conf
sudo chmod 600 /etc/wpa_supplicant/wpa_supplicant.conf

sudo cp wifi-static.sh /usr/bin/wifi-static.sh
sudo chmod 755 /usr/bin/wifi-static.sh

sudo cp wifi-static.service /lib/systemd/system/wifi-static.service
```

2. Reload systemd and enable the service:
```bash
sudo systemctl daemon-reload
sudo systemctl enable wifi-static.service
sudo systemctl start wifi-static.service
```

3. Check status and logs:
```bash
sudo systemctl status wifi-static.service
sudo journalctl -u wifi-static.service -b
```

4. Verify the interface and connectivity:
```bash
ip addr show wlan0
ip route show
cat /etc/resolv.conf
ping -c3 8.8.8.8
```

If anything fails, run the script manually to debug and check `journalctl -xe` and `dmesg`:
```bash
sudo /usr/bin/wifi-static.sh
sudo journalctl -xe
dmesg | tail -n 50
```

---

## Integrating into Yocto (image build)

1. Place recipe and files in your layer:
```
meta-yourlayer/
  recipes-connectivity/
    wifi/
      wifi-static_0.1.bb
      files/
        wpa_supplicant.conf
        wifi-static.sh
        wifi-static.service
```

2. Add your layer to `bblayers.conf` (if not already added):
```bash
bitbake-layers add-layer /path/to/meta-yourlayer
```

3. Ensure the image installs the package: add to `local.conf` or your image recipe:
```
IMAGE_INSTALL_append = " wifi-static"
```

4. Build your image:
```bash
source oe-init-build-env
bitbake your-image-name
```

5. Flash the generated image to SD and boot. Then test with the manual steps above.

---

## Caveats, warnings and best practices

- **Conflicting network managers**: If your distribution uses `dhcpcd`, NetworkManager or connman, do **not** use this script as-is. Instead configure static IP through the distro's network manager (e.g., `/etc/dhcpcd.conf` for dhcpcd or NetworkManager connection files). Using multiple managers on the same interface causes race conditions.
- **DNS value**: Do not use a `.0` network address as the DNS server. Use a valid DNS resolver IP (e.g., `10.76.36.1` or public `8.8.8.8`).
- **File permissions**: Keep `wpa_supplicant.conf` mode 600 to protect your wireless PSK.
- **Network-online target**: Using `network-online.target` increases the likelihood that the network subsystem is ready. For more strict guarantees, use `systemd-networkd-wait-online.service` or equivalent.
- **Debugging**: Use `journalctl -u wifi-static.service -b` and `dmesg` to find hardware or driver errors.
- **Alternative approach**: If you prefer a more "systemd-native" approach, consider shipping `systemd-networkd` unit and `.network` files and have `systemd-networkd-wait-online.service` orchestrate online readiness instead of a custom script.

---

## Included archive structure
The zip archive `wifi-static-package.zip` contains the following structure:

```
wifi-static-package/
  meta-yourlayer/
    recipes-connectivity/
      wifi/
        wifi-static_0.1.bb
        files/
          wpa_supplicant.conf
          wifi-static.sh
          wifi-static.service
  README.md    # this file (same content as file.md)
```

---

## Next steps I can do for you (pick any)
- Update the recipe name/version (e.g., `wifi-static_1.0.bb`) and regenerate the archive.
- Prepare an alternative package that uses `dhcpcd` (edit `/etc/dhcpcd.conf`) instead of the custom script.
- Create a systemd-networkd-based configuration (with `.network` and `.netdev` files) for a cleaner systemd-native solution.
- Create a zip specifically for manual install (flattened files without the Yocto recipe).

Tell me which of the above you'd like next, or I can proceed with all of them.
