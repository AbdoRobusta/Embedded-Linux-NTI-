# Yocto Recipe — Detailed Explanation (reference)

> Purpose: a single-file reference that explains every important part of a simple Yocto recipe.
> Use this as a quick lookup when you return to your GitHub repo later.

---

## Recipes covered (example names)

* `hello-cmake_1.0.bb` — CMake-based example (produces `hello_cmake`)
* `hello-gpp_1.0.bb` — manual g++ compile example (produces `hello_gpp`)

> Both recipes assume sources are shipped with the recipe under `files/` and referenced via `file://` in `SRC_URI`.

---

## 1. Full recipe contents (examples)

### `hello-cmake_1.0.bb`

```bitbake
SUMMARY = "Simple Hello World app built with CMake"
LICENSE = "CLOSED"
SRC_URI = "file://hello.cpp file://CMakeLists.txt"
S = "${WORKDIR}"

inherit cmake

do_install() {
    install -d ${D}${bindir}
    install -m 0755 hello_cmake ${D}${bindir}/hello_cmake
}
```

### `hello-gpp_1.0.bb`

```bitbake
SUMMARY = "Simple Hello World app built using g++ manually"
LICENSE = "CLOSED"
SRC_URI = "file://hello.cpp"
S = "${WORKDIR}"

do_compile() {
    ${CXX} hello.cpp -o hello_gpp
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 hello_gpp ${D}${bindir}/hello_gpp
}
```

---

## 2. Line-by-line explanation — common fields

* `SUMMARY = "..."`
  Short human-readable description shown in tools and search results.

* `LICENSE = "..."`
  The license identifier. For local/closed code you can use `CLOSED`, but for public repos use a real SPDX id (e.g., `MIT`, `GPL-2.0-only`).

* `SRC_URI = "file://..."`
  Where to fetch sources. `file://` means files are in the recipe `files/` directory. Other forms:

  * `git://host/foo.git;branch=master;protocol=git;subdir=...`
  * `http://server/foo.tar.gz;sha256=...`
    Always pin `SRCREV` for git and checksums for tarballs when possible.

* `S = "${WORKDIR}"`
  Directory where sources are located for configure/compile steps. `${WORKDIR}` is the default per-recipe build directory (`tmp/work/<arch>/<pn>/<pv>/temp/...`). If your source unpacks into a subdirectory, override `S` accordingly:

  ```bitbake
  S = "${WORKDIR}/foo-1.2.3"
  ```

* `inherit <class>`
  Inherit standard classes that implement common tasks:

  * `inherit cmake` → provides `do_configure` and `do_compile` using CMake.
  * `inherit autotools` → for `./configure && make` projects.
  * `inherit python3native`, `inherit pkgconfig`, `inherit systemd` etc.
    Inheriting a class saves you from writing `do_configure`/`do_compile` by hand.

* `do_compile()` / `do_install()`
  Task functions you can override. `do_compile()` builds; `do_install()` installs to the pseudo-root `${D}`. Always install into `${D}` paths (not absolute `/`) so packaging can capture the layout.

* `${D}` — pseudo root used for staging install.
  E.g. `install -m 0755 mybin ${D}${bindir}/mybin` puts the binary into the image package.

* `${bindir}`, `${sbindir}`, `${libdir}`, `${sysconfdir}`
  Standard path variables. On target they expand to `/usr/bin`, `/usr/sbin`, `/usr/lib`, `/etc`, etc.

---

## 3. Naming, versions and package names

* Recipe filename format: `<PN>_<PV>.bb` `<PV> Version`
  Example: `hello-cmake_1.0.bb` → `PN = "hello-cmake"`, `PV = "1.0"`.
* If you need revisions: append `-r<N>` (PR) when you make recipe changes, or use `PV` bumping.

---

## 4. Build flow (what Yocto runs for these recipes)

Typical tasks sequence for a recipe:

```
do_fetch -> do_unpack -> do_patch -> do_configure -> do_compile -> do_install -> do_package -> do_deploy
```

For `hello-cmake` with `inherit cmake`:

* `do_fetch` copies files from `files/` (no network)
* `do_configure` runs CMake
* `do_compile` runs `make`
* `do_install` runs the recipe's `do_install` (our custom function)

For `hello-gpp` (manual):

* `do_fetch` copies `hello.cpp`
* `do_configure` is a no-op
* `do_compile` runs the recipe-defined `${CXX}` command
* `do_install` installs the produced binary

---

## 5. Cross-compilation notes

* Use the compiler variables Yocto provides, not hard-coded host tools:

  * `${CC}` for C compiler, `${CXX}` for C++ compiler
  * `${CFLAGS}`, `${CXXFLAGS}`, `${LDFLAGS}` for flags
* Using `${CXX}` ensures the build uses the correct cross-toolchain automatically.
* Do **not** call plain `g++` or system `cmake`/`make` directly unless you understand you are building for the host.

---

## 6. Packaging & inclusion in images

* After installation into `${D}`, BitBake packages files into packages named by `${PN}`.
* To include the package in an image, add to `IMAGE_INSTALL` (either in `local.conf` or your image recipe):

  ```bitbake
  IMAGE_INSTALL:append = " hello-cmake hello-gpp"
  ```
* Alternatively add them to an image recipe:

  ```bitbake
  IMAGE_INSTALL += " hello-cmake hello-gpp "
  ```

---

## 7. Useful BitBake commands (debug & iterative development)

* Build single recipe (full tasks):
  `bitbake hello-cmake`

* Run only specific task:
  `bitbake -c fetch hello-cmake`
  `bitbake -c configure hello-cmake`
  `bitbake -c compile hello-gpp`

* Start a development shell (interactive) inside the recipe workdir:
  `bitbake -c devshell hello-cmake`
  This drops you into the work dir with environment prepared to run `cmake`, `make`, etc.

* Clean recipe state (force rebuild):
  `bitbake -c clean hello-cmake`
  `bitbake -c cleanall hello-cmake` (cleans downloads and sstate references)

* Inspect logs on failure:
  `less tmp/work/<MACHINE>-*/hello-cmake/1.0-r*/temp/log.do_compile.*`

---

## 8. Where to find the produced files during/after build

Within build tree:

* Work directory: `tmp/work/<arch>/<pn>/<pv>/`
* Installed pseudo-root (after `do_install`):
  `tmp/work/<arch>/<pn>/<pv>/image/` or under `${D}` while packaging:
  `tmp/work/<arch>/<pn>/<pv>/image/usr/bin/hello_cmake`

Final image (if included in image recipe):

* `tmp/deploy/images/<MACHINE>/core-image-<name>-<version>.rpi-sdimg` or `.wic`

---

## 9. Good practices & checklist before publishing recipe

* Use real license identifiers and set `LIC_FILES_CHKSUM` where appropriate (for tarballs or license files).
* Pin git revisions with `SRCREV` and include checksums for remote tarballs.
* Add `DEPENDS` if your code requires other recipes at build time (e.g., `DEPENDS = "libfoo"`).
* Use `FILES_${PN}` only if you place files in non-standard locations.
* Test locally: `bitbake -c devshell` + manual `cmake`/`make` to iterate fast.
* Use `inherit gettext` / `inherit pkgconfig` / `inherit systemd` when relevant.

---

## 10. Common recipe variants & extra metadata you may need

* Add runtime dependencies (packages expected to be present on target):

  ```bitbake
  RDEPENDS_${PN} = "libstdc++ busybox"
  ```

* If installing a systemd service: inherit `systemd` and set:

  ```bitbake
  inherit systemd
  SYSTEMD_SERVICE_${PN} = "myservice.service"
  SYSTEMD_AUTO_ENABLE_${PN} = "enable"
  ```

* If you need to bundle config files:

  ```bitbake
  install -d ${D}${sysconfdir}/myapp
  install -m 0644 my.conf ${D}${sysconfdir}/myapp/my.conf
  ```

---

## 11. Quick troubleshooting (most common issues)

* **Binary not found in image**: ensure `do_install()` places binary into `${D}${bindir}` and that package is in `IMAGE_INSTALL`.
* **Cross-compile link errors**: check `${LDFLAGS}` and library dependencies; add required recipes to `DEPENDS`.
* **Permission/ownership issues**: use `install -m` to set permissions and `install -d` to create directories inside `${D}`.
* **Recipe not picked up**: ensure layer containing recipe is included in `bblayers.conf` and folder structure follows `recipes-*/<name>/<name>_<version>.bb`.

---

## 12. Minimal checklist to publish recipe to GitHub

1. Include:

   * `.bb` file(s) in `recipes-*/<your-recipe>/`
   * `files/` folder with source files or patches
   * `README.md` explaining how to add layer and build
   * Proper license file (or `LICENSE` and `LIC_FILES_CHKSUM` entry)
2. Document building steps in `README.md`:

   ```bash
   bitbake <your-recipe>
   # or add to IMAGE_INSTALL and bitbake your image
   ```
3. Note cross-build caveats and Yocto branch compatibility for users.

---

## 13. Example quick reference (copyable snippets)

* Add package to image:

```bitbake
IMAGE_INSTALL:append = " hello-cmake hello-gpp"
```

* Build only recipe:

```bash
bitbake hello-cmake
```

* Open a devshell:

```bash
bitbake -c devshell hello-cmake
```

* Inspect compile log on failure:

```bash
less tmp/work/*/hello-cmake/*/temp/log.do_compile.*
```

---