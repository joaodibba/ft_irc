#!/bin/bash

set -euo pipefail

# Define color codes
RED="\033[1;31m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
BLUE="\033[1;34m"
CYAN="\033[1;36m"
BOLD="\033[1m"
RESET="\033[0m"

OS=$(uname -s)
dependencies=("python3" "pip3")
missing_deps=()

function check_dependencies {
    echo -e "${BLUE}-------------------------------------------------${RESET}"
    echo -e "${BOLD}${CYAN} Checking dependencies...${RESET}"
    echo -e "${BLUE}-------------------------------------------------${RESET}"

    # Clear any previously detected missing dependencies
    missing_deps=()

    for dep in "${dependencies[@]}"; do
        if ! command -v "$dep" &> /dev/null; then
            missing_deps+=("$dep")
        fi
    done

    pip3 install -r requirements.txt

    if [ ${#missing_deps[@]} -ne 0 ]; then
        echo -e "${RED}${BOLD}⚠️ Missing dependencies: ${missing_deps[*]}${RESET}"
        return 1
    else
        echo -e "${GREEN}${BOLD}✅ All dependencies are installed.${RESET}"
        return 0
    fi
}

function install_missing_dependencies {
    echo -e "${BLUE}-------------------------------------------------${RESET}"
    echo -e "${BOLD}${CYAN} Installing missing dependencies...${RESET}"
    echo -e "${BLUE}-------------------------------------------------${RESET}"

    if [[ "$OS" == "Linux" ]]; then
        apt update && apt install -y python3 python3-pip 
    elif [[ "$OS" == "Darwin" ]]; then
        # macOS
        brew install python3
    else
        echo -e "${RED}${BOLD}❌ Unsupported OS: $OS${RESET}"
        exit 1
    fi

    pip3 install -r requirements.txt
    echo -e "${GREEN}${BOLD}✅ Dependencies installed successfully.${RESET}"
}

function prompt_to_install_missing_dependencies {
    echo -e "${YELLOW}${BOLD}Some dependencies are missing. Do you want to install them? (y/n)${RESET}"
    read -r response
    if [[ "$response" =~ ^[Yy]$ ]]; then
        install_missing_dependencies
    else
        echo -e "${RED}${BOLD}Skipping dependency installation.${RESET}"
    fi
}

function prompt_user_server_is_running {
    echo -e "${YELLOW}${BOLD}⚠️ Is your server running? (y/n)${RESET}"
    read -r response
    if [[ "$response" =~ ^[Nn]$ ]]; then
        echo -e "${RED}${BOLD}❌ Please start your server before running tests.${RESET}"
        exit 1
    fi
}

function clean_pycache {
    echo -e "${BLUE}-------------------------------------------------${RESET}"
    echo -e "${BOLD}${CYAN} Cleaning __pycache__ and .pyc files...${RESET}"
    echo -e "${BLUE}-------------------------------------------------${RESET}"
    find . -name "__pycache__" -type d -exec rm -r {} + 2>/dev/null || true
    find . -name "*.pyc" -delete 2>/dev/null || true
    find . -name "*.pyo" -delete 2>/dev/null || true
    echo -e "${GREEN}${BOLD}✅ Cleanup completed.${RESET}"
}

function run_tests {
    echo -e "${BLUE}-------------------------------------------------${RESET}"
    echo -e "${BOLD}${CYAN} Running tests for: ${YELLOW}$1 ${RESET}"
    echo -e "${BLUE}-------------------------------------------------${RESET}"
    echo

    if [ -z "$1" ]; then
        echo -e "${RED}${BOLD}⚠️ No argument passed.${RESET}"
        echo -e "${RED}Please provide a test file or directory to run.${RESET}"
        exit 1
    else
        pytest $1
    fi

    echo
    echo -e "${GREEN}${BOLD}✅ Test execution completed for: ${YELLOW}$1 ${RESET}"
    echo -e "${BLUE}-------------------------------------------------${RESET}"
}

# Main Execution Flow
echo -e "${CYAN}${BOLD}🚀 Starting Test Runner...${RESET}"

if ! check_dependencies; then
    prompt_to_install_missing_dependencies
fi

prompt_user_server_is_running
clean_pycache

run_tests IRC || true
run_tests JOIN || true
run_tests PRIVMSG || true
run_tests USER || true

echo -e "${BLUE}${BOLD}All tests finished!${RESET}"
