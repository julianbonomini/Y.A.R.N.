#!/bin/bash

attempts=0
max_attempts=3
while [ $attempts -lt $max_attempts ]; do
  brew update && brew upgrade
  if [ $? -eq 0 ]; then
    break
  fi
  echo "Homebrew install failed. Retrying in 5 seconds..."
  sleep 5
  attempts=$((attempts+1))
done
if [ $attempts -eq $max_attempts ]; then
  echo "Homebrew install failed after multiple retries."
  exit 1
fi
brew install sfml
brew install nlohmann-json
brew install openssl