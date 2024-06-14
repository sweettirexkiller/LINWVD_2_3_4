#!/bin/sh

# Ścieżka do pliku z aplikacją
APP_PATH="/etc/init.d/appLab4.py"

# Sprawdź, czy Python3 jest zainstalowany
if ! command -v python3 &> /dev/null
then
    echo "Python3 nie jest zainstalowany. Proszę zainstalować Python3."
    exit 1
fi

# Sprawdź, czy Flask jest zainstalowany
if ! python3 -c "import flask" &> /dev/null
then
    echo "Flask nie jest zainstalowany. Proszę zainstalować Flask za pomocą 'pip3 install flask'."
    exit 1
fi

# Uruchom aplikację Flask
echo "Uruchamianie aplikacji Flask..."
python3 $APP_PATH 