#!/bin/bash
echo "🚀 Starting Environment Check..."
if command -v python3 &>/dev/null; then
    echo "✅ Python 3 is installed."
else
    echo "❌ Python 3 is missing."
fi
if command -v g++ &>/dev/null; then
    echo "✅ G++ Compiler is ready."
else
    echo "❌ G++ is missing."
fi