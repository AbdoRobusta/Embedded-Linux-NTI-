#!/bin/sh
# /usr/bin/wifi-static.sh
# Configure WiFi with static IP on wlan0 (improved)
set -e

CONF="/etc/wpa_supplicant/wpa_supplicant.conf"
IP="10.76.36.250"
PREFIX="24"           # e.g., 24 for 255.255.255.0
GATEWAY="10.76.36.1"
DNS="10.76.36.1"      # change to your DNS (or 8.8.8.8)

echo "[*] Starting WiFi static setup"

# Unblock WiFi (rfkill)
rfkill unblock wifi || true
rfkill unblock all || true

# Bring interface up
ip link set wlan0 up || true
sleep 1

# Start wpa_supplicant if not running for wlan0
if ! pgrep -f "wpa_supplicant.*wlan0" >/dev/null 2>&1; then
    killall wpa_supplicant 2>/dev/null || true
    wpa_supplicant -B -i wlan0 -c "$CONF" || true
    sleep 3
fi

# Wait for interface to exist
for i in $(seq 1 10); do
    if ip link show wlan0 >/dev/null 2>&1; then
        break
    fi
    echo "[*] Waiting for wlan0..."
    sleep 1
done

# Disable WiFi power saving (stability)
iw dev wlan0 set power_save off 2>/dev/null || true

# Flush old addresses and assign static IP
ip addr flush dev wlan0 || true
ip addr add ${IP}/${PREFIX} dev wlan0

# Set default gateway
ip route add default via ${GATEWAY} dev wlan0 2>/dev/null || true

# Configure DNS (overwrite resolv.conf)
if [ -n "${DNS}" ]; then
    printf "nameserver %s\n" "${DNS}" > /etc/resolv.conf
fi

echo "[+] WiFi static setup complete"
