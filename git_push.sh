#!/bin/bash

echo "⚙️  Adding all changes..."
git add .

echo "💬 Commit message (press ENTER to use default):"
read msg

if [ -z "$msg" ]; then
  msg="Update project"
fi

git commit -m "$msg"

echo "🚀 Pushing to GitHub..."
git push origin main

echo "✅ Push complete!"
