#!/bin/bash

echo "⚙️  Adding all changes..."
git add .

echo "💬 Commit message (press ENTER to use default):"
read msg

if [ -z "$msg" ]; then
  msg="Update project"
fi

git commit -m "$msg"

echo "🔄 Pulling latest changes from GitHub..."
if ! git pull origin main --rebase; then
  echo "❌ Error: git pull failed. Please resolve conflicts and try again."
  exit 1
fi

echo "🚀 Pushing to GitHub..."
if ! git push origin main; then
  echo "❌ Error: git push failed. Please check your network or repository permissions."
  exit 1
fi

echo "✅ Push complete!"
