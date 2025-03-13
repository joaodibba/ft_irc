#!/bin/bash

set -euo pipefail

OS=$(uname -s)
dependencies=("python3" "pip3")
missing_deps=()

function check_dependencies {
    echo "Checking dependencies..."
    # Clear any previously detected missing dependencies
    missing_deps=()

    for dep in "${dependencies[@]}"; do
        if ! command -v "$dep" &> /dev/null; then
            missing_deps+=("$dep")
        fi
    done

    pip3 install -r requirements.txt

    if [ ${#missing_deps[@]} -ne 0 ]; then
        echo "Missing dependencies: ${missing_deps[*]}"
        return 1
    else
        echo "All dependencies are installed."
        return 0
    fi
}

function install_missing_dependencies {
    echo "Installing missing dependencies..."

    if [[ "$OS" == "Linux" ]]; then
        apt update && apt install -y python3 python3-pip 
    elif [[ "$OS" == "Darwin" ]]; then
        # macOS
        brew install python3
    else
        echo "Unsupported OS: $OS"
        exit 1
    fi

    pip3 install -r requirements.txt
}

function prompt_to_install_missing_dependencies {
    echo "Some dependencies are missing. Do you want to install them? (y/n)"
    read -r response
    if [[ "$response" =~ ^[Yy]$ ]]; then
        install_missing_dependencies
    else
        echo "Skipping dependency installation."
    fi
}

function prompt_user_server_is_running {
    echo "Is your server running? (y/n)"
    read -r response
    if [[ "$response" =~ ^[Nn]$ ]]; then
        echo "Please start your server before running tests."
        exit 1
    fi
}

function run_tests {
    echo "Running tests for $1..."
    # check if any value is passed as argument
    if [ -z "$1" ]; then
        echo "No argument passed. Please pass the test file name to run."
    else
        pytest $1
    fi
}

# Main Execution Flow

# We use '|| true' to prevent the script from exiting immediately if check_dependencies fails,
# so we can handle missing dependencies
if ! check_dependencies; then
    prompt_to_install_missing_dependencies
fi

prompt_user_server_is_running
run_tests IRC
run_tests JOIN
run_tests PRIVMSG
run_tests USER