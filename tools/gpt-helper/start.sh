#!/bin/bash

# Create the virtual environment
python3 -m venv chat_term

# Activate the virtual environment
source chat_term/bin/activate

# Install requirements
pip install -r requirements.txt
