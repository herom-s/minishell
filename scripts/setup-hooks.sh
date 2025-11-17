#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${YELLOW}[INFO]${NC} Setting up Git hooks for minishell..."

# Check if .git directory exists
if [ ! -d ".git" ]; then
    echo -e "${RED}[ERROR]${NC} Not a git repository. Please run this script from the project root."
    exit 1
fi

# Check for required tools
FORMATTER_INSTALLED=false
NORMINETTE_INSTALLED=false

if command -v c_formatter_42 &> /dev/null; then
    FORMATTER_INSTALLED=true
    echo -e "${GREEN}[OK]${NC} c_formatter_42 found"
else
    echo -e "${YELLOW}[WARNING]${NC} c_formatter_42 not found"
fi

if command -v norminette &> /dev/null; then
    NORMINETTE_INSTALLED=true
    echo -e "${GREEN}[OK]${NC} norminette found"
else
    echo -e "${YELLOW}[WARNING]${NC} norminette not found"
fi

# Create hooks directory if it doesn't exist
mkdir -p .git/hooks

# Create commit-msg hook
COMMIT_MSG_HOOK=".git/hooks/commit-msg"

cat > "$COMMIT_MSG_HOOK" << 'EOF'
#!/bin/sh

# Commit message format validator
# Expected format: <type>(<scope>): <description>
# 
# Valid types: feat, fix, docs, style, refactor, test, chore, perf, ci, build, revert
# Scope is optional but recommended
#
# Examples:
#   feat(parser): add support for pipes
#   fix(builtins): correct cd behavior with relative paths
#   docs: update README with installation instructions

commit_msg=$(cat "$1")

# Allow merge commits
if echo "$commit_msg" | grep -qE "^Merge"; then
    exit 0
fi

# Check if commit message matches the conventional commit pattern
if ! echo "$commit_msg" | grep -qE "^(feat|fix|docs|style|refactor|test|chore|perf|ci|build|revert)(\([a-z0-9_-]+\))?: .+$"; then
    echo ""
    echo "❌ [ERROR] Invalid commit message format!"
    echo ""
    echo "Expected format: <type>(<scope>): <description>"
    echo ""
    echo "Valid types:"
    echo "  feat     - A new feature"
    echo "  fix      - A bug fix"
    echo "  docs     - Documentation changes"
    echo "  style    - Code style changes (formatting, etc.)"
    echo "  refactor - Code refactoring"
    echo "  test     - Adding or updating tests"
    echo "  chore    - Maintenance tasks"
    echo "  perf     - Performance improvements"
    echo "  ci       - CI/CD changes"
    echo "  build    - Build system changes"
    echo "  revert   - Revert a previous commit"
    echo ""
    echo "Scope is optional but recommended (e.g., parser, lexer, builtins)"
    echo ""
    echo "✅ Valid examples:"
    echo "  feat(parser): add support for pipes"
    echo "  fix(builtins): correct cd behavior with relative paths"
    echo "  docs: update README with installation instructions"
    echo "  refactor(executor): simplify process management"
    echo "  test(lexer): add tokenization tests"
    echo ""
    echo "📝 Your commit message:"
    echo "  $commit_msg"
    echo ""
    exit 1
fi

# Check description length (should be descriptive)
description=$(echo "$commit_msg" | sed -E 's/^[a-z]+(\([a-z0-9_-]+\))?: //')
if [ ${#description} -lt 3 ]; then
    echo ""
    echo "⚠️  [WARNING] Commit description is too short (minimum 3 characters)"
    echo ""
    exit 1
fi

# Check if description starts with lowercase
if ! echo "$description" | grep -qE "^[a-z]"; then
    echo ""
    echo "⚠️  [WARNING] Description should start with a lowercase letter"
    echo ""
    exit 1
fi

# Check if description ends with a period
if echo "$description" | grep -qE "\.$"; then
    echo ""
    echo "⚠️  [WARNING] Description should not end with a period"
    echo ""
    exit 1
fi

exit 0
EOF

chmod +x "$COMMIT_MSG_HOOK"

echo -e "${GREEN}[OK]${NC} commit-msg hook installed"

# Create pre-commit hook for formatting and norminette
PRE_COMMIT_HOOK=".git/hooks/pre-commit"

cat > "$PRE_COMMIT_HOOK" << 'EOF'
#!/bin/sh

# Pre-commit hook for formatting and norminette

echo "🔍 Running pre-commit checks..."

# Check if c_formatter_42 is available
if command -v c_formatter_42 &> /dev/null; then
    echo "📝 Formatting C files with c_formatter_42..."
    
    # Get list of staged C files
    STAGED_C_FILES=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(c|h)$')
    
    if [ -n "$STAGED_C_FILES" ]; then
        for file in $STAGED_C_FILES; do
            if [ -f "$file" ]; then
                echo "  Formatting: $file"
                c_formatter_42 "$file"
                git add "$file"
            fi
        done
        echo "✅ C files formatted successfully"
    else
        echo "ℹ️  No C files to format"
    fi
else
    echo "⚠️  c_formatter_42 not found - skipping formatting"
    echo "    Install with: pip3 install --user 42-formatter"
fi

# Run norminette if available
if command -v norminette &> /dev/null; then
    echo "🔎 Running norminette..."
    
    # Get list of staged C files
    STAGED_C_FILES=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(c|h)$')
    
    if [ -n "$STAGED_C_FILES" ]; then
        norminette $STAGED_C_FILES
        if [ $? -ne 0 ]; then
            echo ""
            echo "❌ Norminette failed. Please fix the errors before committing."
            echo ""
            exit 1
        fi
        echo "✅ Norminette passed"
    fi
else
    echo "⚠️  norminette not found - skipping norm check"
    echo "    Install with: pip3 install --user norminette"
fi

echo "✅ Pre-commit checks complete!"
exit 0
EOF

chmod +x "$PRE_COMMIT_HOOK"

echo -e "${GREEN}[OK]${NC} pre-commit hook installed"

# Summary
echo ""
echo -e "${GREEN}✨ Git hooks setup complete!${NC}"
echo ""
echo "Installed hooks:"
echo "  - commit-msg: Validates conventional commit format"
echo "  - pre-commit: Formats C files and runs norminette"
echo ""

# Show tool status
echo "Development tools status:"
if [ "$FORMATTER_INSTALLED" = true ]; then
    echo -e "  ${GREEN}✓${NC} c_formatter_42 - installed"
else
    echo -e "  ${RED}✗${NC} c_formatter_42 - not installed"
    echo -e "    ${BLUE}→${NC} pip3 install --user 42-formatter"
fi

if [ "$NORMINETTE_INSTALLED" = true ]; then
    echo -e "  ${GREEN}✓${NC} norminette - installed"
else
    echo -e "  ${RED}✗${NC} norminette - not installed"
    echo -e "    ${BLUE}→${NC} pip3 install --user norminette"
fi

echo ""

if [ "$FORMATTER_INSTALLED" = false ] || [ "$NORMINETTE_INSTALLED" = false ]; then
    echo -e "${YELLOW}[INFO]${NC} Some tools are missing. Install them for full functionality:"
    echo ""
    if [ "$FORMATTER_INSTALLED" = false ]; then
        echo "  pip3 install --user 42-formatter"
    fi
    if [ "$NORMINETTE_INSTALLED" = false ]; then
        echo "  pip3 install --user norminette"
    fi
    echo ""
fi
