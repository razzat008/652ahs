!/bin/bash
install_debian_packages() {
    echo "Installing packages for Debian-based systems..."
    sudo apt update || { echo "Failed to update package list"; exit 2; }
    sudo apt install -y mesa-utils xorg-dev libxkbcommon-dev libgl1-mesa-dev libglu1-mesa-dev libglfw3-dev libsqlite3-dev || { echo "Failed to install packages"; exit 3; }
    echo "Installation successful"
}

install_arch_packages() {
    echo "Installing packages for Arch-based systems..."
    sudo pacman -S --noconfirm --needed sqlite3 glfw mesa mesa-utils || { echo "Failed to install packages"; exit 3; }
    echo "Installation successful"
}

if [ "$(id -u)" != "0" ]; then
    echo "This script must be run with root privileges"
    exit 1
fi

if [ ! -f /etc/os-release ] || [ ! -r /etc/os-release ]; then
    echo "Unable to detect the operating system"
    exit 4
fi

if command -v apt &> /dev/null; then
        install_debian_packages
elif command -v pacman &> /dev/null; then
        install_arch_packages
else
    echo "Unsupported package manager"
    exit 1
fi
