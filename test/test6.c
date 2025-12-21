#include <stdio.h>

//sajuidabfiboiashfnDFB;iodfblks
//vSJKBDULVKBI;FSKV
//soigfhirsjg09s
dfosivnafds;vn
aEOFPSIUDBV;Skip to content
        Navigation Menu
        google-gemini
        gemini-cli

        Type / to search
Code
        Issues
1.8k
Pull requests
510
Discussions
        Actions
Projects
1
Security
        Insights
Owner avatar
gemini-cli
        Public
google-gemini/gemini-cli
        Go to file
t
        Name
gemini-cli-robot
        gemini-cli-robot
        chore/release: bump version to 0.21.0-nightly.20251220.41a1a3eed (#15352
8643d60
·
8 hours ago
.allstar
        Downgrade branch_protection to log (#2934)
4 months ago
.gcp
        Fix cloudbuild step. (#7131)
4 months ago
.gemini-clipboard
        feat: simplify tool confirmation labels for better UX (#15296)
2 days ago
.gemini
        Add core tool that adds all context from the core package. (#15238)
3 days ago
.github
(fix) Automated pr labeller (#14885)
4 days ago
.husky
fix(pre-commit): improve monorepo handling and failure messaging (#9123)
3 months ago
.vscode
cleanup(markdown): Prettier format all markdown @ 80 char width (#10714)
2 months ago
docs
        Introspection agent demo (#15232)
2 days ago
integration-tests
        Simplify method signature. (#15114)
last week
packages
        chore/release: bump version to 0.21.0-nightly.20251220.41a1a3eed (#15352
8 hours ago
schemas
        Introspection agent demo (#15232)
2 days ago
scripts
        Introspection agent demo (#15232)
2 days ago
third_party/get-ripgrep
        Fix Windows ripgrep detection (#11492)
2 months ago
.editorconfig
        chore: add .editorconfig (#2572)
6 months ago
.gitattributes
        Adding .gitattributes (#1303)
6 months ago
.gitignore
fix(ui): show command suggestions even on perfect match and sort them (…
2 days ago
.lycheeignore
        Fix broken links in documentation (#11789)
2 months ago
.npmrc
        fix: prepublish changes to package names (#1420)
6 months ago
.nvmrc
        chore: Expand node version test matrix (#2700)
5 months ago
.prettierignore
feat(mcp): Inject GoogleCredentialProvider headers in McpClient (#13783)
last month
.prettierrc.json
cleanup(markdown): Prettier format all markdown @ 80 char width (#10714)
2 months ago
.yamllint.yml
        Release Process vNext (#8152)
3 months ago
CONTRIBUTING.md
chore(docs): add 'Maintainers only' label info to CONTRIBUTING.md (#1…
last week
Dockerfile
        Revert "Update package.json in include git dependency on Ink fork." (#…
2 months ago
GEMINI.md
        Add docs directive to GEMINI.md (#14327)
3 weeks ago
LICENSE
        Revert to default LICENSE (Revert #13449) (#13876)
3 weeks ago
Makefile
        Fix duplicate help text for build targets in Makefile (#4092)
5 months ago
README.md
        Remove old E2E Workflows (#14749)
2 weeks ago
ROADMAP.md
cleanup(markdown): Prettier format all markdown @ 80 char width (#10714)
2 months ago
SECURITY.md
cleanup(markdown): Prettier format all markdown @ 80 char width (#10714)
2 months ago
esbuild.config.js
        Inline tree-sitter wasm and add runtime fallback (#11157)
2 months ago
eslint.config.js
        Disallow unnecessary awaits. (#15172)
4 days ago
package-lock.json
        chore/release: bump version to 0.21.0-nightly.20251220.41a1a3eed (#15352
8 hours ago
package.json
        chore/release: bump version to 0.21.0-nightly.20251220.41a1a3eed (#15352
8 hours ago
tsconfig.json
        Explict imports & exports with type modifier (#3774)
4 months ago
Repository files navigation
        README
Contributing
        Apache-2.0 license
        Security
Gemini CLI
Gemini CLI CI Gemini CLI E2E (Chained) Version License View Code Wiki

        Gemini CLI Screenshot

Gemini CLI is an open-source AI agent that brings the power of Gemini directly into your terminal. It provides lightweight access to Gemini, giving you the most direct path from your prompt to our model.

Learn all about Gemini CLI in our documentation.

🚀 Why Gemini CLI?
🎯 Free tier: 60 requests/min and 1,000 requests/day with personal Google account.
🧠 Powerful Gemini 2.5 Pro: Access to 1M token context window.
🔧 Built-in tools: Google Search grounding, file operations, shell commands, web fetching.
🔌 Extensible: MCP (Model Context Protocol) support for custom integrations.
💻 Terminal-first: Designed for developers who live in the command line.
🛡️ Open source: Apache 2.0 licensed.
📦 Installation
        Pre-requisites before installation
        Node.js version 20 or higher
macOS, Linux, or Windows
Quick Install
Run instantly with npx
# Using npx (no installation required)
npx https://github.com/google-gemini/gemini-cli
Install globally with npm
npm install -g @google/gemini-cli
        Install globally with Homebrew (macOS/Linux)
brew install gemini-cli
        Release Cadence and Tags
        See Releases for more details.

Preview
        New preview releases will be published each week at UTC 2359 on Tuesdays. These releases will not have been fully vetted and may contain regressions or other outstanding issues. Please help us test and install with preview tag.

npm install -g @google/gemini-cli@preview
        Stable
New stable releases will be published each week at UTC 2000 on Tuesdays, this will be the full promotion of last week's preview release + any bug fixes and validations. Use latest tag.
npm install -g @google/gemini-cli@latest
        Nightly
New releases will be published each day at UTC 0000. This will be all changes from the main branch as represented at time of release. It should be assumed there are pending validations and issues. Use nightly tag.
npm install -g @google/gemini-cli@nightly
📋 Key Features
Code Understanding & Generation
        Query and edit large codebases
Generate new apps from PDFs, images, or sketches using multimodal capabilities
        Debug issues and troubleshoot with natural language
Automation & Integration
        Automate operational tasks like querying pull requests or handling complex rebases
Use MCP servers to connect new capabilities, including media generation with Imagen, Veo or Lyria
        Run non-interactively in scripts for workflow automation
Advanced Capabilities
Ground your queries with built-in Google Search for real-time information
Conversation checkpointing to save and resume complex sessions
Custom context files (GEMINI.md) to tailor behavior for your projects
GitHub Integration
Integrate Gemini CLI directly into your GitHub workflows with Gemini CLI GitHub Action:

Pull Request Reviews: Automated code review with contextual feedback and suggestions
Issue Triage: Automated labeling and prioritization of GitHub issues based on content analysis
        On-demand Assistance: Mention @gemini-cli in issues and pull requests for help with debugging, explanations, or task delegation
        Custom Workflows: Build automated, scheduled and on-demand workflows tailored to your team's needs
🔐 Authentication Options
Choose the authentication method that best fits your needs:

Option 1: Login with Google (OAuth login using your Google Account)
✨ Best for: Individual developers as well as anyone who has a Gemini Code Assist License. (see quota limits and terms of service for details)

Benefits:

Free tier: 60 requests/min and 1,000 requests/day
        Gemini 2.5 Pro with 1M token context window
        No API key management - just sign in with your Google account
        Automatic updates to latest models
Start Gemini CLI, then choose Login with Google and follow the browser authentication flow when prompted
        gemini
If you are using a paid Code Assist License from your organization, remember to set the Google Cloud Project
# Set your Google Cloud Project
export GOOGLE_CLOUD_PROJECT="YOUR_PROJECT_ID"
gemini
        Option 2: Gemini API Key
✨ Best for: Developers who need specific model control or paid tier access

Benefits:

Free tier: 100 requests/day with Gemini 2.5 Pro
        Model selection: Choose specific Gemini models
Usage-based billing: Upgrade for higher limits when needed
# Get your key from https://aistudio.google.com/apikey
export GEMINI_API_KEY="YOUR_API_KEY"
gemini
        Option 3: Vertex AI
✨ Best for: Enterprise teams and production workloads

        Benefits:

Enterprise features: Advanced security and compliance
Scalable: Higher rate limits with billing account
Integration: Works with existing Google Cloud infrastructure
# Get your key from Google Cloud Console
export GOOGLE_API_KEY="YOUR_API_KEY"
export GOOGLE_GENAI_USE_VERTEXAI=true
gemini
        For Google Workspace accounts and other authentication methods, see the authentication guide.

🚀 Getting Started
Basic Usage
Start in current directory
gemini
        Include multiple directories
gemini --include-directories ../lib,../docs
        Use specific model
gemini -m gemini-2.5-flash
        Non-interactive mode for scripts
        Get a simple text response:

gemini -p "Explain the architecture of this codebase"
For more advanced scripting, including how to parse JSON and handle errors, use the --output-format json flag to get structured output:

gemini -p "Explain the architecture of this codebase" --output-format json
For real-time event streaming (useful for monitoring long-running operations), use --output-format stream-json to get newline-delimited JSON events:

gemini -p "Run tests and deploy" --output-format stream-json
        Quick Examples
        Start a new project
        cd new-project/
gemini
> Write me a Discord bot that answers questions using a FAQ.md file I will provide
        Analyze existing code
git clone https://github.com/google-gemini/gemini-cli
cd gemini-cli
        gemini
> Give me a summary of all of the changes that went in yesterday
📚 Documentation
        Getting Started
        Quickstart Guide - Get up and running quickly.
Authentication Setup - Detailed auth configuration.
Configuration Guide - Settings and customization.
Keyboard Shortcuts - Productivity tips.
Core Features
Commands Reference - All slash commands (/help, /chat, etc).
Custom Commands - Create your own reusable commands.
Context Files (GEMINI.md) - Provide persistent context to Gemini CLI.
Checkpointing - Save and resume conversations.
Token Caching - Optimize token usage.
Tools & Extensions
        Built-in Tools Overview
        File System Operations
Shell Commands
Web Fetch & Search
        MCP Server Integration - Extend with custom tools.
Custom Extensions - Build and share your own commands.
Advanced Topics
Headless Mode (Scripting) - Use Gemini CLI in automated workflows.
Architecture Overview - How Gemini CLI works.
IDE Integration - VS Code companion.
Sandboxing & Security - Safe execution environments.
Trusted Folders - Control execution policies by folder.
Enterprise Guide - Deploy and manage in a corporate environment.
Telemetry & Monitoring - Usage tracking.
Tools API Development - Create custom tools.
Local development - Local development tooling.
Troubleshooting & Support
        Troubleshooting Guide - Common issues and solutions.
FAQ - Frequently asked questions.
Use /bug command to report issues directly from the CLI.
Using MCP Servers
        Configure MCP servers in ~/.gemini/settings.json to extend Gemini CLI with custom tools:

> @github List my open pull requests
> @slack Send a summary of today's commits to #dev channel
> @database Run a query to find inactive users
See the MCP Server Integration guide for setup instructions.

🤝 Contributing
        We welcome contributions! Gemini CLI is fully open source (Apache 2.0), and we encourage the community to:

Report bugs and suggest features.
Improve documentation.
Submit code improvements.
Share your MCP servers and extensions.
See our Contributing Guide for development setup, coding standards, and how to submit pull requests.

Check our Official Roadmap for planned features and priorities.

📖 Resources
        Official Roadmap - See what's coming next.
Changelog - See recent notable updates.
NPM Package - Package registry.
GitHub Issues - Report bugs or request features.
Security Advisories - Security updates.
Uninstall
        See the Uninstall Guide for removal instructions.

📄 Legal
        License: Apache License 2.0
Terms of Service: Terms & Privacy
        Security: Security Policy
Built with ❤️ by Google and the open source community

        About
An open-source AI agent that brings the power of Gemini directly into your terminal.

geminicli.com
        Topics
cli ai gemini ai-agents gemini-api mcp-server mcp-client
        Resources
Readme
        License
Apache-2.0 license
        Contributing
Contributing
        Security policy
        Security policy
        Activity
Custom properties
Stars
88.2k stars
        Watchers
461 watching
        Forks
10.1k forks
        Report repository
        Releases 265
Release v0.21.3
Latest
2 days ago
+ 264 releases
        Used by 338
@Replicant-Partners
@oktho1114-debug
@VSKurkin
@4xiaxia
@JSTONE1111
@Dmelleis
@screamingearth
@robert2687
+ 330
Contributors
416
@jacob314
@scidomino
@NTaylorMullen
@mattKorwel
@olcan
@abhipatel12
@SandyTao520
@allenhutchison
@skeshive
@gemini-code-assist[bot]
@chrstnb
@gemini-cli-robot
@KeijiBranshi
@galz10
+ 402 contributors
        Deployments
491
prod 11 hours ago
dev last month
        github-pages
+ 488 deployments
        Languages
TypeScript
98.1%

JavaScript
1.9%
Footer
© 2025 GitHub, Inc.
Footer navigation
Terms
        Privacy
Security
        Status
Community
        Docs
Contact
        Manage cookies
        Do not share my personal information
        Repository search resultsBENIGN_RACE_ENDafn;iauerv