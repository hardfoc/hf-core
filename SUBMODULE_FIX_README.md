# CI Build Failure Fix - Config Parser in Submodule Scripts

## Problem Summary
The ESP32 Examples Build CI workflow (run ID: 22011748464) failed due to a config parsing issue in the `examples/esp32/scripts` submodule.

**Error Messages:**
```text
Warning: yq not found. Falling back to basic parsing.
ERROR: Could not extract build types from config
ERROR: Invalid build type: Debug
Available types: 
```

## Root Cause Analysis

The failure occurs in `examples/esp32/scripts/config_loader.sh` (from the `N3b3x/hf-espidf-project-tools` submodule) when:

1. **Insufficient grep context**: The fallback parser uses `grep -A 10 "metadata:"` but `build_types:` appears 13 lines after `metadata:` in the YAML file
2. **Multiline YAML format incompatibility**: The parser expects inline arrays like `build_types: [...]` but the YAML uses multiline format:
   ```yaml
   build_types:
       - ["Debug", "Release"]
       - ["Debug"]
   ```

## Solution Applied

The fix modifies `config_loader.sh` with two key changes:

### 1. Increased grep context window
Changed all instances of `grep -A 10 "metadata:"` to `grep -A 20 "metadata:"` to ensure all fields are captured.

### 2. Implemented proper multiline YAML parsing
Replaced single-line grep/sed parsing with section-based parsing:
```bash
# Old approach (fails with multiline):
local build_line=$(grep -A 10 "metadata:" "$CONFIG_FILE" | grep "build_types:")
content=$(echo "$build_line" | sed 's/.*build_types: *\[//' | sed 's/\].*//')

# New approach (handles multiline):
metadata_section=$(sed -n '/^metadata:/,/^[a-z]/p' "$CONFIG_FILE" | head -n -1)
build_types_section=$(echo "$metadata_section" | sed -n '/^  build_types:/,/^  [a-z]/p' | head -n -1)
echo "$build_types_section" | grep "^ *-" | sed 's/^ *- *//' | ...
```

### Functions Updated
- `load_config_basic()` - Basic config loading
- `get_build_types()` - Get all build types
- `get_build_types_for_idf_version()` - Get build types for specific IDF version
- `get_idf_versions()` - Get list of IDF versions
- `get_app_build_types_for_idf_version()` - Get app-specific build types

## Testing Results

The fix was validated to correctly parse:
- Build types: `Debug Release`
- IDF versions: `release/v5.5 release/v5.4`
- Build types for `release/v5.5`: `Debug Release`
- Build types for `release/v5.4`: `Debug`

All parsing functions work correctly in the fallback mode (without yq).

## Files Modified

### Submodule: examples/esp32/scripts (N3b3x/hf-espidf-project-tools)
- `config_loader.sh` - 79 insertions, 47 deletions

The complete patch is available in:
- `0001-fix-handle-multiline-YAML-format-in-config-parser-fa.patch`
- `submodule-fix.patch` (identical copy)

## Implementation Status

✅ Fix developed and tested
✅ Patch file created
✅ Committed to local submodule branch: `fix/config-parser-multiline-yaml`
⏳ **Requires**: Push to `N3b3x/hf-espidf-project-tools` repository
⏳ **Requires**: Update submodule reference in `hf-core` main repository

## Next Steps

### Option 1: Apply Patch to Submodule Repository (Recommended)
1. Push the fix to `N3b3x/hf-espidf-project-tools`:
   ```bash
   cd examples/esp32/scripts
   git push origin fix/config-parser-multiline-yaml
   ```
2. Create and merge PR in the submodule repository
3. Update the submodule reference in `hf-core`:
   ```bash
   cd examples/esp32/scripts
   git checkout main
   git pull
   cd ../..
   git add examples/esp32/scripts
   git commit -m "chore: update scripts submodule with config parser fix"
   ```

### Option 2: Apply Patch File
If direct push is not possible, apply the patch file in the submodule repository:
```bash
cd <hf-espidf-project-tools-repo>
git apply /path/to/submodule-fix.patch
git commit -m "fix: handle multiline YAML format in config parser fallback"
```

## Verification

After applying the fix, the CI should pass with these log messages:
```
Warning: yq not found. Falling back to basic parsing.
Build types: Debug Release
IDF versions: release/v5.5 release/v5.4
```

No errors should appear during config parsing.
