---
layout: default
title: At25040Handler
parent: Handlers
nav_order: 15
---

# At25040Handler

SPI EEPROM handler for Microchip AT25010/020/040B. Wraps `at25040::Device<>`
over `BaseSpi` with an optional `/WP` `BaseGpio`. Construction is cheap —
`Probe()` is the first bus traffic. Enable with `HF_CORE_ENABLE_AT25040`.

Product identity parsing (AID1 / Flux accessory EEPROM) lives in
`AccessoryIdentityManager`, not here.

Future AFE parts (PGA, DAC) follow the same split: `hf-<part>-driver` +
`handlers/<part>/` + a domain manager owner. Do not put handlers under
`lib/managers/afe/`.
