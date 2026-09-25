# ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore Module: mod-dangerous-nights

[![AzerothCore Module](https://img.shields.io/badge/AzerothCore-Module-red?style=flat-square&logo=github)](https://github.com/azerothcore/azerothcore-wotlk)
[![C++20](https://img.shields.io/badge/Language-C++20-00599C?style=flat-square&logo=c%2B%2B)](https://isocpp.org/)
[![Branch 3.3.5a](https://img.shields.io/badge/Branch-3.3.5a-orange?style=flat-square)](https://github.com/azerothcore/azerothcore-wotlk)
[![License AGPL v3](https://img.shields.io/badge/License-AGPL%20v3-blue?style=flat-square)](LICENSE)
[![GitHub Stars](https://img.shields.io/github/stars/AlsoNotMehh/mod-dangerous-nights?style=flat-square&color=yellow&logo=github)](https://github.com/AlsoNotMehh/mod-dangerous-nights/stargazers)

A dynamic nighttime loot and reward enhancement module for **AzerothCore (WotLK 3.3.5a)**.

When night falls across Azeroth, hunting monsters becomes significantly more lucrative: players receive increased experience rewards, extra gold, and bonus item drops from night encounters.

---

## 🔗 Companion Modules

This module was created to work directly alongside the following companion modules:

- 🌅 **[`mod-fast-day-night`](https://github.com/AlsoNotMehh/mod-fast-day-night):** Accelerates the world day/night cycle into a fast 4-hour day / 4-hour night progression, allowing players to experience frequent daylight and nighttime hunting cycles throughout each play session.
- 💀 **[`mod-hardcore-scaling`](https://github.com/AlsoNotMehh/mod-hardcore-scaling):** Dynamically tunes creature damage and health scaling during night hours (or general schedules) to provide challenging combat for hardcore realms.

---

## ✨ Features

- 🪙 **Bonus Gold:** Increases gold looted from creatures during nighttime (+50% by default).
- ✨ **Bonus Experience:** Grants increased experience for slaying monsters at night (+50% by default).
- 🎁 **Bonus Item Drops:** Extra percentage chance to roll additional item loot on night kills (+20% by default).
- 📢 **Day/Night Announcements:** Optional server broadcast when night begins and when dawn breaks.

---

## ⚙️ Configuration (`mod_dangerous_nights.conf`)

| Setting | Default | Description |
| :--- | :---: | :--- |
| `DangerousNights.Enable` | `1` | Enable or disable the module. |
| `DangerousNights.NightStartHour` | `20` | Hour when night begins (default: 8:00 PM). |
| `DangerousNights.NightEndHour` | `6` | Hour when night ends (default: 6:00 AM). |
| `DangerousNights.FastClockSpeed` | `0.0` | Speed multiplier (`3.0` when used with `mod-fast-day-night`). |
| `DangerousNights.XPMultiplier` | `1.50` | Experience reward multiplier at night (+50%). |
| `DangerousNights.GoldMultiplier` | `1.50` | Gold looted multiplier at night (+50%). |
| `DangerousNights.ExtraLootChance` | `20.0` | Percentage chance for bonus item drops at night. |
| `DangerousNights.AllowInDungeons` | `0` | Enable in dungeons (`0` = outdoor world only). |
| `DangerousNights.AnnounceTransitions` | `1` | Announce dusk and dawn transitions. |

---

## 📦 Installation

1. Clone into your AzerothCore `modules` directory:
   ```bash
   cd azerothcore/modules
   git clone https://github.com/AlsoNotMehh/mod-dangerous-nights.git
   ```
2. Re-run CMake and compile your server:
   ```bash
   cmake -B build
   cmake --build build --config Release
   ```
3. Copy `conf/mod_dangerous_nights.conf.dist` to your `configs` directory as `mod_dangerous_nights.conf`.

---

## 📄 License

This project is licensed under the [GNU AGPL v3 License](LICENSE).
