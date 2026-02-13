---
layout: default
title: Utilities
nav_order: 5
has_children: true
---

# Utility Libraries

hf-core includes three utility libraries providing reusable infrastructure:

| Library | Description | Header Count |
|:--------|:------------|:------------|
| [General Utilities](general_utilities) | Data structures, algorithms, filters | ~30 headers |
| [RTOS Wrappers](rtos_wrappers) | FreeRTOS C++ abstractions | ~22 headers |
| [CANopen](canopen) | CAN frame + protocol helpers | 3 headers |

All utilities are designed to be:

- **Platform-portable** — General utilities are header-only with no RTOS dependency
- **Thread-safe** — RTOS wrappers manage synchronization internally
- **Zero-allocation** — Fixed-size containers, no dynamic memory in hot paths
