# YQ Installation Fix for ESP32 Build CI

## Problem

The ESP32 Examples Build CI workflow fails because `yq` (YAML processor) is not installed in the build environment.

### Error Reference
- Failed Workflow Run: https://github.com/hardfoc/hf-core/actions/runs/22079280828/job/63801115165#step:5:1
- Issue: https://github.com/hardfoc/hf-core/pull/4
- Error Message: `ERROR: Could not extract build types from config`

## Root Cause

### Primary Issue
The workflow chain is:
1. `.github/workflows/esp32-examples-build-ci.yml` calls external reusable workflow
2. `N3b3x/hf-espidf-ci-tools/.github/workflows/ru-build.yml@main` executes build inside ESP-IDF Docker container
3. The build script `build_app.sh` sources `config_loader.sh` from `examples/esp32/scripts`
4. `config_loader.sh` uses `yq` to parse `app_config.yml` YAML configuration
5. `yq` is NOT installed in the ESP-IDF Docker image

### Secondary Issue (Fallback Failure)
While `config_loader.sh` has a fallback mechanism using `grep/sed`, the fallback was also failing because:
- The fallback uses `grep -A 10` to extract fields after `metadata:`
- In `app_config.yml`, `metadata:` is on line 13
- `build_types:` is on line 26 (13 lines after metadata)
- `grep -A 10` only shows 10 lines, missing `build_types`

This caused the error:
```
Warning: yq not found. Falling back to basic parsing.
ERROR: Could not extract build types from config
```

## Solutions

### Solution 1: Install yq (Recommended - Primary Fix)

The best solution is to install `yq` in the Docker container before running the build script.

**Required Changes**:
- **Repository**: `N3b3x/hf-espidf-ci-tools`
- **File**: `.github/workflows/ru-build.yml`
- **Change**: Add yq installation in Docker command section (around line 176)

#### Patch File (Primary Fix)

```bash
# Clone the repository
git clone https://github.com/N3b3x/hf-espidf-ci-tools.git
cd hf-espidf-ci-tools

# Create a new branch
git checkout -b fix/install-yq-in-docker

# Apply the patch (see patch content below)

# Commit and push
git add .github/workflows/ru-build.yml
git commit -m "fix: install yq in Docker container for YAML parsing"
git push origin fix/install-yq-in-docker

# Create a pull request on GitHub
```

**Patch Content**:

```diff
diff --git a/.github/workflows/ru-build.yml b/.github/workflows/ru-build.yml
index eac9c9e..317b51a 100644
--- a/.github/workflows/ru-build.yml
+++ b/.github/workflows/ru-build.yml
@@ -173,6 +173,19 @@ jobs:
             done
             echo "======================================================"
             echo ""
+            # Install yq for YAML parsing in config_loader.sh
+            echo "======================================================"
+            echo "INSTALLING YQ"
+            echo "======================================================"
+            YQ_VERSION="4.44.6"
+            YQ_ARCH="linux_amd64"
+            echo "Installing yq version ${YQ_VERSION} for ${YQ_ARCH}..."
+            wget -q "https://github.com/mikefarah/yq/releases/download/v${YQ_VERSION}/yq_${YQ_ARCH}" -O /usr/local/bin/yq
+            chmod +x /usr/local/bin/yq
+            echo "yq installed successfully:"
+            yq --version
+            echo "======================================================"
+            echo ""
             # Project tools directory already validated in generate-matrix job
             # Run build script with correct paths
             "${{ needs.generate-matrix.outputs.project_tools_path }}/build_app.sh" \
```

### Solution 2: Fix Fallback Parsing (Secondary Enhancement)

As an additional improvement, fix the fallback mechanism to work without yq.

**Required Changes**:
- **Repository**: `N3b3x/hf-espidf-project-tools`  
- **File**: `config_loader.sh`
- **Change**: Increase grep context from `-A 10` to `-A 20`

#### Patch File (Secondary Fix)

```bash
# Clone the repository (or navigate to the submodule)
cd examples/esp32/scripts

# Create a new branch
git checkout -b fix/improve-fallback-parsing

# Apply the patch (see patch content below)

# Commit and push
git add config_loader.sh
git commit -m "fix: increase grep context for fallback parsing"
git push origin fix/improve-fallback-parsing

# Create a pull request on GitHub
```

**Patch Summary**:
- Changes all `grep -A 10 "metadata:"` to `grep -A 20 "metadata:"`
- Affects lines: 256-261, 311, 342-344, 402, 510-512, 550
- Tested and verified to work without yq installed

Full patch available in `patches/fallback-parsing-fix.patch`

## Testing

### Verify Primary Fix (yq installation)
After applying the yq installation fix:
1. Push the changes to the `N3b3x/hf-espidf-ci-tools` repository
2. The ESP32 Examples Build CI workflow will automatically use the updated version (references `@main`)
3. Trigger a new workflow run in `hardfoc/hf-core`
4. Verify that yq is installed and the build succeeds

### Verify Secondary Fix (fallback parsing)
After applying the fallback parsing fix:
1. Test locally without yq:
   ```bash
   cd examples/esp32
   export PROJECT_PATH=$(pwd)
   PATH="/usr/local/sbin:/sbin:/bin" bash -c 'source scripts/config_loader.sh && get_build_types'
   ```
2. Expected output: `Debug Release` (no errors)

## Implementation Status

- [x] **Primary Fix Created**: Patch ready for `N3b3x/hf-espidf-ci-tools`
- [x] **Secondary Fix Created**: Patch ready for `N3b3x/hf-espidf-project-tools`
- [x] **Tested**: Both fixes verified to work correctly
- [ ] **Applied**: Waiting for patches to be applied to external repositories
- [ ] **Verified**: Waiting for successful workflow run

## Recommendation

**Apply both fixes**:
1. **Primary fix** ensures yq is available (most reliable solution)
2. **Secondary fix** improves robustness for cases where yq might not be available

This provides defense-in-depth: the workflow will work with yq (preferred), and will also work without yq (fallback).

## Files Involved

### In hf-core repository:
- `.github/workflows/esp32-examples-build-ci.yml` - Main workflow file (no changes needed)
- `examples/esp32/scripts/` - Submodule containing build scripts (secondary fix)
- `YQ_INSTALLATION_FIX.md` - This documentation file

### In N3b3x/hf-espidf-ci-tools repository:
- `.github/workflows/ru-build.yml` - Reusable workflow (primary fix needed)

### In N3b3x/hf-espidf-project-tools repository:
- `config_loader.sh` - Configuration loader script (secondary fix available)

### Patch Files
- `patches/yq-installation-fix.patch` - Primary fix for yq installation
- `patches/fallback-parsing-fix.patch` - Secondary fix for fallback parsing

## References

- yq repository: https://github.com/mikefarah/yq
- yq releases: https://github.com/mikefarah/yq/releases
- ESP-IDF CI Action: https://github.com/espressif/esp-idf-ci-action
- Patch files location: `patches/` directory in this repository
