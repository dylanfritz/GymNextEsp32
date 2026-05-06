Got you. Here is the **clean, copy‑paste‑ready Markdown**, with **no extra chatter**, **no UI elements**, and **every command grouped, labeled, and formatted** so you can drop it straight into a README or reference doc.

---

# GymNext Timer BLE Command Reference

_All commands extracted from the provided source. Each entry includes prefix, purpose, and an example._

---

## 1. System / Device Control

| Prefix | Example | Description |
|-------|---------|-------------|
| `_F` | `_F` | Factory reset |
| `BL` | `BL` | Restart in bootloader mode |
| `P1` | `P1` | Power on |
| `P0` | `P0` | Power off |
| `P2` | `P2` | Restart |
| `NM?` | `NM?MyName` | Set device name |
| `VS?` | `VS?1234` | Verify secure code |
| `SS?` | `SS?1234` | Set secure code |
| `SN` | `SN` | Secure mode: None |
| `SA` | `SA` | Secure mode: Admin |
| `SL` | `SL` | Secure mode: All |
| `TZ?` | `TZ?-5,00` | Set timezone offset |
| `DT?` | `DT?-1,184` | Send device time (initial handshake) |
| `XX` | `XX` | Initial communication token |
| `CA?` | `CA?-1,184` | Calibration command |

---

## 2. Clock & Display Modes

| Prefix | Example | Description |
|--------|---------|-------------|
| `CL` | `CL` | Display mode: Clock |
| `TI` | `TI` | Display mode: Timer |
| `ME` | `ME` | Display mode: Message |
| `WE` | `WE` | Display mode: Welcome (hardware > 1) |
| `TE` | `TE` | Display mode: Test |
| `_C` | `_C` | Clear display |
| `@H` | `@H` | Toggle 12h/24h |
| `H1` | `H1` | 12h clock on |
| `H0` | `H0` | 12h clock off |
| `@E` | `@E` | Toggle show seconds |
| `E1` | `E1` | Show seconds on |
| `E0` | `E0` | Show seconds off |

---

## 3. Audio / Buzzer

| Prefix | Example | Description |
|--------|---------|-------------|
| `M1` | `M1` | Mute on |
| `M0` | `M0` | Mute off |
| `@M` | `@M` | Toggle mute |
| `ZZ?` | `ZZ?1` | Buzz on/off |
| `ZB?` | `ZB?3` | Beep pattern |
| `ZR?` | `ZR?120` | Raw buzzer value |
| `WW?` | `WW?…` | Warning strategy (software ≥ 22) |
| `WT?` | `WT?…` | Warning tone (software ≥ 22) |
| `WN?` | `WN?…` | Warning noise (software ≥ 22) |

---

## 4. Display Settings

| Prefix | Example | Description |
|--------|---------|-------------|
| `BR?` | `BR?3` | Brightness (older hardware) |
| `BT?` | `BT?5` | Brightness (hardware v2) |
| `BF?` | `BF?1` | Brightness checking frequency |
| `ER?` | `ER?1` | Error checking frequency |
| `BU?` | `BU?1` | On-screen buffer enable |
| `CE?` | `CE?1` | Colon pulse enable |
| `I1` | `I1` | Show connection indicator on |
| `I0` | `I0` | Show connection indicator off |
| `@I` | `@I` | Toggle connection indicator |
| `J1` | `J1` | Show sleep indicator on |
| `J0` | `J0` | Show sleep indicator off |
| `@J` | `@J` | Toggle sleep indicator |

---

## 5. Timer Control

| Prefix | Example | Description |
|--------|---------|-------------|
| `_S` | `_S` | Start |
| `_P` | `_P` | Pause |
| `_R` | `_R` | Reset |
| `XT?` | `XT?50,500` | Seed timer (ceil/10, raw ms) |
| `XC?` | `XC?$$$$` | Seed clock (UTC seconds) |
| `PR?` | `PR?10` | Prelude duration |
| `IT?` | `IT?5` | Inactivity timeout |

---

## 6. Direction / Continuity / Segue

| Prefix | Example | Description |
|--------|---------|-------------|
| `@D` | `@D` | Toggle direction |
| `D1` | `D1` | Direction up |
| `D0` | `D0` | Direction down |
| `@C` | `@C` | Toggle continuity |
| `C1` | `C1` | Continuity on |
| `C0` | `C0` | Continuity off |
| `@S` | `@S` | Toggle segue |
| `S1` | `S1` | Segue on |
| `S0` | `S0` | Segue off |
| `B1` | `B1` | Rest direction override up |
| `B0` | `B0` | Rest direction override down |
| `BX` | `BX` | Remove rest direction override |

---

## 7. Sleep Mode

| Prefix | Example | Description |
|--------|---------|-------------|
| `SM?` | `SM?10,20` | Set sleep window |
| `SE?` | `SE?20` | Sleep end |
| `SJ?` | `SJ?10` | Sleep start |

---

## 8. Messages & Text

| Prefix | Example | Description |
|--------|---------|-------------|
| `XM?` | `XM?HELLO` | Set message |
| `XR?` | `XR?1,2,3` | Raw message bytes |
| `WM?` | `WM?WELCOME` | Set welcome text |
| `WR?` | `WR?1,2,3` | Raw welcome bytes |
| `FM?` | `FM?3,HELLO` | Flash message |
| `FR?` | `FR?3,1,2,3` | Flash message raw |
| `ST?` | `ST?READY` | Custom status |
| `SR?` | `SR?1,2,3` | Custom status raw |

---

## 9. Schedules & Intervals

| Prefix | Example | Description |
|--------|---------|-------------|
| `SC?` | `SC?5,10,3,...` | Interval schedule |
| `SF?` | `SF?…` | Full schedule (extended) |
| `IN` | `IN` | Status mode: Interval |
| `IL` | `IL` | Status mode: Repetition |
| `CU` | `CU` | Status mode: Custom |
| `NO` | `NO` | Status mode: None |
| `MT?` | `MT?120` | Metronome (software ≥ 22) |

---

## 10. Hardware Profile

| Prefix | Example | Description |
|--------|---------|-------------|
| `HP?` | `HP?1,0,2,1,0` | Hardware profile (display, size, colors) |
| `SB?` | `SB?1` | Buzzer type |

---

## 11. Legacy Buzz / Warn

| Prefix | Example | Description |
|--------|---------|-------------|
| `ZZ?0` | `ZZ?0` | Legacy warn/clack |

---

## 12. Special Tokens

| Token | Description |
|--------|-------------|
| `$$$$` | Replaced with seconds since midnight UTC |
| `####` | Replaced with adjusted seed time |

---

If you want, I can also generate:

- a **compact cheat sheet**,  
- a **JSON schema**,  
- or a **code‑ready enum + lookup table** for your SDK.

Just tell me what format you want next.
