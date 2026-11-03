"""Name-based mapping of an error code onto ``hf::Status``.

Shared by ``tools/gen_iiw_status.py`` (every ``hf_*_err_t`` in hf-internal-interface-wrap) and
``tools/new_handler.py`` (a driver's ``enum class Error``). Both write the result out as an explicit
``switch`` with no ``default``, so the compiler's ``-Wswitch`` flags a code that was added later, and a
reviewer reads the mapping in the generated header, not here.

Names are matched in UPPER_SNAKE form: ``InvalidChipId`` and ``INVALID_CHIP_ID`` are the same code. The
interface prefix (``SPI_ERR_``, ``I2C_``) is removed before matching. The first rule that matches wins;
a name no rule matches is ``Fault``.
"""

from __future__ import annotations

import re

STATUSES = (
    "Ok",
    "NotInitialized",
    "InvalidArgument",
    "Busy",
    "Timeout",
    "Bus",
    "NotFound",
    "Fault",
    "Unsupported",
    "DependencyNotReady",
    "NoMemory",
    "Unknown",
)

# Exact names (after the prefix is removed) whose meaning the keyword rules below would get wrong.
EXACT = {
    "OK": "Ok",
    "NONE": "Ok",
    "SUCCESS": "Ok",
    "UNKNOWN": "Unknown",
    "MAX": "Unknown",  # an enum's count sentinel, never returned
    "COUNT": "Unknown",
    "FAIL": "Fault",
    "FAILURE": "Fault",
    "TIMEOUT_ALT": "Timeout",
    "INVALID_STATE": "Busy",  # called in the wrong state: retry once the state changes
    "INVALID_READING": "Fault",
    "INVALID_DATA": "Fault",
    "INVALID_CALIBRATION": "Fault",
    "CALIBRATION_INVALID": "Fault",
    "OUT_OF_RANGE": "Fault",  # a reading out of range, not an argument
    "SENSOR_NOT_AVAILABLE": "NotFound",
    "NO_CALLBACK": "DependencyNotReady",
    "GATT_ERROR": "Bus",
    "NOT_CALIBRATED": "DependencyNotReady",
    "CALIBRATION_NOT_FOUND": "DependencyNotReady",
    "INVALID_CHIP_ID": "NotFound",
    "DEVICE_ID_MISMATCH": "NotFound",
    "CHIP_ID_MISMATCH": "NotFound",
    "WRONG_CHIP_ID": "NotFound",
    "BUS_ARBITRATION_LOST": "Bus",
    "DEVICE_NOT_RESPONDING": "Bus",
    "DEVICE_NACK": "Bus",
    "POR_SOFT_RESET": "Fault",
    "READ_ONLY": "Unsupported",
    "FILTER_FULL": "NoMemory",
}

# (pattern, status), tried in order on the UPPER_SNAKE name. Patterns are regular expressions matched
# anywhere in the name, on word boundaries made of "_".
RULES = (
    (r"NOT_INITIALI[SZ]ED|NOT_STARTED|NOT_ENABLED|NOT_RUNNING|NOT_CONNECTED|CHANNEL_DISABLED|SENSOR_DISABLED"
     r"|UNINITIALI[SZ]ED", "NotInitialized"),
    (r"UNSUPPORTED|NOT_SUPPORTED|PERMISSION_DENIED|ACCESS_DENIED|SECURITY_VIOLATION", "Unsupported"),
    (r"OUT_OF_MEMORY|NO_MEMORY|INSUFFICIENT_RESOURCES|INSUFFICIENT_CHANNELS|STORAGE_FULL|NO_FREE_PAGES|NO_MEM", "NoMemory"),
    (r"TIMEOUT|TIMED_OUT", "Timeout"),
    (r"NOT_FOUND", "NotFound"),
    (r"BUS_NOT_AVAILABLE|RESOURCE_UNAVAILABLE|NOT_AVAILABLE|NOT_CONFIGURED|DEPENDENCY", "DependencyNotReady"),
    (r"BUSY|ALREADY_|QUEUE_FULL|BUFFER_FULL|FIFO_FULL|_EMPTY$|OPERATION_PENDING|MAX_CONNECTIONS|NOT_READY"
     r"|IN_PROGRESS",
     "Busy"),
    (r"BUS_ERROR|BUS_OFF|BUS_RECOVERY|ARBITRATION|NACK|TRANSFER_FAILED|COMMUNICATION|NOT_RESPONDING"
     r"|READ_FAILURE|WRITE_FAILURE|READ_FAILED|WRITE_FAILED|READ_ERR|WRITE_ERR|FRAME_ERROR|PARITY|OVERRUN|NOISE"
     r"|BREAK_DETECTED|TRANSMISSION|RECEPTION|^TX_|^RX_|STUFF_ERROR|FORM_ERROR|CRC|ACK_ERROR|BIT_ERROR"
     r"|CHECKSUM|MESSAGE_LOST|CS_CONTROL|TRANSCEIVER|SEND_FAILED|DMA_ERROR|^BUS$|SPI_ERROR|I2C_ERROR"
     r"|UART_ERROR|PROTOCOL", "Bus"),
    (r"INVALID|NULL_POINTER|TOO_LONG|TOO_LARGE|TOO_SMALL|TOO_HIGH|TOO_LOW|TOO_SHORT|TOO_NARROW|TOO_WIDE"
     r"|DUTY_OUT_OF_RANGE|SIZE_MISMATCH|CONFLICT|BAD_ARG|BAD_PARAM|OUT_OF_BOUNDS|FORMAT_ERROR|ENCODING_ERROR",
     "InvalidArgument"),
)


def snake(name: str) -> str:
    """``InvalidChipId`` -> ``INVALID_CHIP_ID``; ``SPI_ERR_BUS_BUSY`` stays as it is."""
    if name.isupper() or "_" in name:
        return name.upper()
    return re.sub(r"(?<=[a-z0-9])(?=[A-Z])|(?<=[A-Z])(?=[A-Z][a-z])", "_", name).upper()


def strip_prefix(name: str, prefix: str = "") -> str:
    """Remove an interface prefix such as ``SPI_ERR_`` or ``SPI_`` (``SPI_SUCCESS``)."""
    if prefix:
        for p in (prefix + "_ERR_", prefix + "_"):
            if name.startswith(p):
                return name[len(p):]
    return name


def classify(name: str, prefix: str = "") -> str:
    """The ``hf::Status`` enumerator for error code @p name."""
    key = strip_prefix(snake(name), prefix)
    if key in EXACT:
        return EXACT[key]
    for pattern, status in RULES:
        if re.search(pattern, key):
            return status
    return "Fault"
