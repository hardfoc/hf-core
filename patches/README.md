# Patches for External Repository Fixes

This directory contains patch files that need to be applied to external repositories to fix the yq installation issue.

## Patch Files

### 1. yq-installation-fix.patch
- **Target Repository**: `N3b3x/hf-espidf-ci-tools`
- **Target File**: `.github/workflows/ru-build.yml`
- **Purpose**: Install yq in Docker container before running build scripts
- **Priority**: **HIGH** (Primary fix)

### 2. fallback-parsing-fix.patch
- **Target Repository**: `N3b3x/hf-espidf-project-tools`
- **Target File**: `config_loader.sh`
- **Purpose**: Fix fallback YAML parsing when yq is not available
- **Priority**: **MEDIUM** (Secondary enhancement)

## How to Apply

### Applying yq-installation-fix.patch

```bash
# Clone the target repository
git clone https://github.com/N3b3x/hf-espidf-ci-tools.git
cd hf-espidf-ci-tools

# Create a new branch
git checkout -b fix/install-yq-in-docker

# Apply the patch
git apply /path/to/yq-installation-fix.patch

# Or apply manually from the patch file
# Then commit
git add .github/workflows/ru-build.yml
git commit -m "fix: install yq in Docker container for YAML parsing"

# Push and create PR
git push origin fix/install-yq-in-docker
```

### Applying fallback-parsing-fix.patch

```bash
# Clone the target repository
git clone https://github.com/N3b3x/hf-espidf-project-tools.git
cd hf-espidf-project-tools

# Create a new branch
git checkout -b fix/improve-fallback-parsing

# Apply the patch
git apply /path/to/fallback-parsing-fix.patch

# Or apply manually from the patch file
# Then commit
git add config_loader.sh
git commit -m "fix: increase grep context for fallback parsing"

# Push and create PR
git push origin fix/improve-fallback-parsing
```

## Testing

After applying the patches:

1. **Test yq installation fix**: Trigger a workflow run in hardfoc/hf-core
2. **Test fallback fix**: Run locally without yq:
   ```bash
   cd examples/esp32
   export PROJECT_PATH=$(pwd)
   PATH="/bin:/sbin" bash -c 'source scripts/config_loader.sh && get_build_types'
   ```

## Related Documentation

See `YQ_INSTALLATION_FIX.md` in the repository root for comprehensive documentation.
