# GnuBox for Symbian S60 3rd Edition (FP2 / Nokia E5-00 / C5-00) - Vetera-Bridge Edition

A modernized port of **GnuBox** for Symbian S60 3rd Edition Feature Pack 2 (specifically tested on Nokia E5-00, RM-632), customized to connect over Bluetooth Serial (RFCOMM / `BTCOMM::5`) + PPP to an ESP32 running [arifwn/vetera-bridge](https://github.com/arifwn/vetera-bridge).

---

## Features & Improvements

1. **Native Symbian 9.3 CommsDat v1.1 Architecture:**
   - Rewritten CommDb management using `CMDBSession` (Central Repository `0xCCCCCC00`).
   - Clean, in-place record modification (`ModifyL`) to prevent `KErrAlreadyExists (-11)` and `KErrInUse (-14)` errors.
   - Protected template records against corruption (`KErrLocked -22`).

2. **Full Compatibility with `vetera-bridge`:**
   - Pre-configured for `BTCOMM::5` (Bluetooth Serial port).
   - PPP protocol configuration using `nullagt.agt` without requiring physical PSTN/GSM dialing.
   - Baud rate fixed at 115,200 bps with 8N1 framing and CTS handshaking.

3. **Packet Data / GPRS Access Point Mapping:**
   - Running `Options -> Set to GPRS` registers an `OUTGOING_WCDMA` access point pointing to the Bluetooth RFCOMM modem.
   - Access point **"Bt"** becomes directly visible in the phone's **Destinations / Access Points** (`Settings -> Connectivity -> Destinations`) menu.
   - When an inactive or dead SIM card is inserted (keeping the phone in General/Normal profile), the Nokia Web Browser can select **"Bt"** directly to load `http://192.168.7.1`.

4. **Modernized S60v3 FP2 UI & Streamlined Menu:**
   - Clean dark-theme interface dynamically sized for 320×240 (landscape, e.g., E5-00 / E71 / E72) and 240×320 (portrait) screens using crisp `LatinPlain12` / `LatinBold12` system fonts.
   - Real-time status card (`Status`, `AP Name`, `Modem`, `Gateway`) and built-in Quick Guide panel.
   - Flattened **Options** menu containing only essential actions (`Bring up IF`, `2box Serial`, `Create records`, `Set to GPRS`, `Delete records`, `Exit`).

---

## Directory Structure

```text
├── aif/            # Application Information File and icons
├── data/           # Resources (gnubox.rss, gnubox_reg.rss, gnubox.mbm)
├── group/          # Project build files (bld.inf, gnubox.mmp, build_s60v3.bat)
├── include/        # C++ header files (.h, .hrh)
├── release/        # Ready-to-install self-signed package (.sisx)
├── sis/            # Package specification (.pkg) and self-signing keys
├── src/            # C++ source files (.cpp)
├── COPYING.txt     # GNU General Public License v2
└── README.md       # Project documentation
```

---

## How to Build

### Requirements
- **OS:** Windows XP / 7 / 10 / 11
- **SDK:** S60 3rd Edition FP2 SDK v1.1 (`C:\S60\devices\S60_3rd_FP2_SDK_v1.1`)
- **Compiler:** GCCE (CSL Arm Toolchain)
- **Tools:** Symbian `makesis` and `signsis`

### Build Steps
1. Open Command Prompt (`cmd.exe`).
2. Navigate to the `group` directory:
   ```cmd
   cd /d path\to\gnubox-s60v3-vetera-git\group
   ```
3. Run the automated build script:
   ```cmd
   build_s60v3.bat
   ```
4. The output package `gnubox_s60v3_vetera.sisx` will be generated in `sis/`.

---

## Usage Guide (with Dead / Inactive SIM)

1. **Insert SIM:** Insert any inactive or dead SIM into the Nokia E5-00 / C5-00 (ensuring the phone boots in **General** profile, not Offline mode).
2. **Install:** Transfer and install `release/gnubox_s60v3_vetera.sisx` on the phone.
3. **Configure CommDb & Pair Bluetooth:**
   - Open **GnuBox**.
   - `Options -> Create records` (confirmation: *"Records created OK!"*).
   - `Options -> 2box Serial` -> select your ESP32 (`vetera-bridge`) Bluetooth device (choose *"No"* on the encryption prompt unless required) to bind it to `BTCOMM::5`.
   - `Options -> Set to GPRS` (confirmation: *"Set to GPRS OK! Access Point 'Bt' visible"*).
4. **Verify Access Point:**
   - Go to `Menu -> Settings -> Connectivity -> Destinations -> Internet`.
   - Verify that the access point **"Bt"** is listed (if **"Bt"** is in `Uncategorised`, move it to `Internet`).
5. **Test & Connect:**
   - Power on the ESP32 running `vetera-bridge`.
   - Optionally select `Options -> Bring up IF` in GnuBox to test the PPP link and HTTP connectivity to `192.168.7.1`.
   - Open the Nokia Web Browser $\rightarrow$ navigate to `http://192.168.7.1` and select connection **"Bt"**!

---

## Credits & License

- Original GnuBox by **Mal Minhas** (GNU General Public License v2).
- Symbian CommsDat v1.1 port and `vetera-bridge` adaptation by the Antigravity community.
- Built for pairing with [arifwn/vetera-bridge](https://github.com/arifwn/vetera-bridge).
