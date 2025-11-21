#!/bin/bash

# Parent directory where all repos live
PARENT_DIR="$(dirname "$PWD")"

# Repo name (folder name) used to make a unique venv per repo
REPO_NAME="$(basename "$PWD")"

# External venv path unique per repo
EXTERNAL_VENV_PATH="$PARENT_DIR/venv/venv_$REPO_NAME"

# Bootstrap activation script inside the repo
BOOTSTRAP_FILE="./venv"

echo "Cleaning old bootstrap file if any"
if [ -e "$BOOTSTRAP_FILE" ]; then
    rm -rf "$BOOTSTRAP_FILE"
    echo "Removed old bootstrap file."
fi

echo "Checking virtual environment location at: $EXTERNAL_VENV_PATH"
if [ ! -d "$EXTERNAL_VENV_PATH" ]; then
    echo "Creating new virtual environment at: $EXTERNAL_VENV_PATH"
    python3 -m venv "$EXTERNAL_VENV_PATH"
else
    echo "Virtual environment already exists."
fi

echo "Installing requirements if available"
if [ -f "requirements.txt" ]; then
    source "$EXTERNAL_VENV_PATH/bin/activate"
    pip install --upgrade pip setuptools wheel
    pip install -r requirements.txt
    deactivate
else
    echo "requirements.txt not found, skipping pip install."
fi

echo "Creating bootstrap script: $BOOTSTRAP_FILE"
cat > "$BOOTSTRAP_FILE" <<'EOF'
#!/bin/bash
# Activates the external virtual environment located at ../venv_<repo_name>

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_NAME="$(basename "$SCRIPT_DIR")"
VENV_PATH="$(dirname "$SCRIPT_DIR")/venv/venv_$REPO_NAME"

if [ ! -f "$VENV_PATH/bin/activate" ]; then
    echo "Virtual environment not found at $VENV_PATH"
    echo "Run ./setup_venv.sh to create it."
    return 1 2>/dev/null || exit 1
fi

source "$VENV_PATH/bin/activate"
EOF

chmod +x "$BOOTSTRAP_FILE"

echo ""
echo "Done."
echo "To activate the virtual environment, run:"
echo "source ./venv"
