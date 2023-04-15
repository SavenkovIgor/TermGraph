# Contribution Guide 💻

Hello there! 👋 I'm so excited to see that you're interested in contributing to TermGraph. As the sole developer of this project, I appreciate all the help I can get to improve TermGraph and make it the go-to tool for creating term diagrams and creating a brief overwiew of some knowledge areas.

To make sure we're all on the same page, I've put together this guide to cover some important topics regarding contributions. Please take a few minutes to go through it 🚀

## Diagram Errors and Fixes 🔍

If you come across any errors or inconsistencies in the diagrams, please don't hesitate to create a Issue or Pull request with the fix. Just follow the pull request procedure mentioned below, and provide a short explanation for your changes. If the error is obvious, no explanation is necessary. Your input is invaluable in making TermGraph the best it can be!

## Coding Style Guidelines 🧹

Maintaining a consistent code style is essential for readability and easier collaboration. So here few simple guidelines:

- Indentation: Please use 4 spaces for indentation
- Function names: Use camelCase for naming functions. For example, calculateNodePosition
- Class names: Use PascalCase for naming classes. For example, GraphRenderer
- Constants: Use UPPER_SNAKE_CASE for constants. For example, MAX_NODE_COUNT

## Pull Request Procedure 🌟

If you find an error in the static graph data in termgraph.app or a missing feature, please follow these steps to create a pull request:

1. **Fork the repository:** Fork the repository to your GitHub account. This will create a copy of the project in your account, which you can work on without affecting the original repository.
2. **Create a new branch:** Create a new branch with a descriptive name in your forked repository. This will help keep the changes organized and make it easier for me to review your contributions. For example, use branch names like fix-node-definition or update-static-graph-data.
3. **Commit your changes:** Make the necessary changes in your branch, following the coding style guidelines mentioned earlier. Be sure to add clear and informative commit messages that describe the changes you've made.
4. **Create a pull request:** Once you're confident in your changes, create a pull request from your branch to the original TermGraph repository. Provide a short description of the changes you made and why you think they're necessary. This will help me understand the context and intent behind your contribution. Wait for GitHub Actions to finish. It runs CTest, so you can check if all other stuff is working as expected. 
5. **Static graph changes:** If you're updating the static graph data (anything from lib/StaticDataStorage/data/*.json), keep in mind that the GitHub workflow will run CTest, which would validate the JSON files in this folder.

By following this procedure, you'll ensure a smooth and organized review process, making it easier for me to understand and integrate your contributions. Thank you for your attention to detail! 🌟

That's it! Thank you so much for taking the time to go through this guide. Your contributions are greatly appreciated. Happy coding! 🎉
