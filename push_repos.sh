#!/usr/bin/env bash

# git branch to check which branch you're working on, git checkout [branch name] to switch branches


# Set the branches you want to push
BRANCH2="main"  # GitHub branch
BRANCH1="dev"   # Gitea branch

# Check if BRANCH2 exists locally, create it if it doesn't
if ! git rev-parse --verify $BRANCH2 >/dev/null 2>&1; then
  echo "Branch $BRANCH2 does not exist locally. Creating it..."
  git checkout -b $BRANCH2
  git push origin $BRANCH2
  git push alt $BRANCH2
else
  echo "Branch $BRANCH2 exists locally. Proceeding with push..."
fi

# Ensure we are on the correct branch before pushing
echo "Switching to branch $BRANCH1..."
git checkout $BRANCH1
echo "Pushing $BRANCH1 to origin..."
git push origin $BRANCH1
if [ $? -ne 0 ]; then
  echo "Failed to push to origin. Please check your branch and remote URL."
  exit 1
fi

echo "Switching to branch $BRANCH2..."
git checkout $BRANCH2
echo "Pushing $BRANCH2 to alt..."
git push alt $BRANCH2
if [ $? -ne 0 ]; then
  echo "Failed to push to alt. Please check your branch and remote URL."
  exit 1
fi

echo "Push to both repositories completed."
