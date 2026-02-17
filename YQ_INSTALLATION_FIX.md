# YQ Installation Fix for ESP32 Build CI

## Problem

The ESP32 Examples Build CI workflow fails because `yq` (YAML processor) is not installed in the build environment.

### Error Reference
- Failed Workflow Run: https://github.com/hardfoc/hf-core/actions/runs/22079280828/job/63801115165#step:5:1
- Issue: https://github.com/hardfoc/hf-core/pull/4

## Root Cause

The workflow chain is:
1. `.github/workflows/esp32-examples-build-ci.yml` calls external reusable workflow
2. `N3b3x/hf-espidf-ci-tools/.github/workflows/ru-build.yml@main` executes build inside ESP-IDF Docker container
3. The build script `build_app.sh` sources `config_loader.sh` from `examples/esp32/scripts`
4. `config_loader.sh` uses `yq` to parse `app_config.yml` YAML configuration
5. `yq` is NOT installed in the ESP-IDF Docker image, causing the build to fail

While `config_loader.sh` has a fallback mechanism using `grep/sed`, it appears the fallback is either not working correctly or the configuration requires `yq` for proper parsing.

## Solution

The fix must be applied to the external reusable workflow repository:
**N3b3x/hf-espidf-ci-tools**

### Required Changes

Install `yq` in the Docker container before running the build script.

#### File to Modify
`.github/workflows/ru-build.yml`

#### Change Location
Add the yq installation step in the `command:` section, right before calling `build_app.sh` (around line 176).

#### Patch File

A patch file has been created that can be applied to the external repository:

```bash
# Clone the repository
git clone https://github.com/N3b3x/hf-espidf-ci-tools.git
cd hf-espidf-ci-tools

# Create a new branch
git checkout -b fix/install-yq-in-docker

# Apply the patch (see patch content below)
# Make the changes manually or use git apply

# Commit and push
git add .github/workflows/ru-build.yml
git commit -m "fix: install yq in Docker container for YAML parsing"
git push origin fix/install-yq-in-docker

# Create a pull request on GitHub
```

### Patch Content

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

### Implementation Details

The fix adds a new section that:
1. Downloads `yq` v4.44.6 (latest stable version) for Linux AMD64
2. Installs it to `/usr/local/bin/yq` (in the Docker container's PATH)
3. Makes it executable
4. Verifies the installation by showing the version

This ensures that when `config_loader.sh` attempts to use `yq`, it is available and functional.

## Testing

After applying the fix:
1. Push the changes to the `N3b3x/hf-espidf-ci-tools` repository
2. The ESP32 Examples Build CI workflow in this repository will automatically use the updated version (since it references `@main`)
3. Trigger a new workflow run to verify the fix works

## Alternative Solutions Considered

1. **Fix fallback parsing in config_loader.sh**: While the script has fallback mechanism, ensuring `yq` is available is more reliable and future-proof.

2. **Fork the reusable workflow**: Creating a fork under `hardfoc` organization would give immediate control but requires ongoing maintenance.

3. **Install yq in project setup**: Not feasible because the build runs inside a Docker container that's managed by the reusable workflow.

## Next Steps

1. Apply the patch to `N3b3x/hf-espidf-ci-tools` repository
2. Verify the fix with a test workflow run
3. Close this issue once confirmed working

## Files Involved

### In hf-core repository:
- `.github/workflows/esp32-examples-build-ci.yml` - Main workflow file (no changes needed)
- `examples/esp32/scripts/` - Submodule containing build scripts (no changes needed)

### In N3b3x/hf-espidf-ci-tools repository:
- `.github/workflows/ru-build.yml` - Reusable workflow that needs the fix

## References

- yq repository: https://github.com/mikefarah/yq
- yq releases: https://github.com/mikefarah/yq/releases
- ESP-IDF CI Action: https://github.com/espressif/esp-idf-ci-action
