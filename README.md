# Kronos
Kronos is a worklog tracker for Jira. It's a work in progress.

# Features
- Native application for Linux, MacOS and Windows. No silly web interfaces.
- Jira integration
- Visual representation of worklogs
- Keeps track of recently used Jira issues
- Happy color scheme to brighten your day

# Screenshot
![Kronos screenshot](kronos-screenshot.jpg)

# Build status
![Unit-test workflow](https://github.com/llob/kronos/actions/workflows/unit-tests.yml/badge.svg)
![CodeQL workflow](https://github.com/llob/kronos/actions/workflows/codeql-analysis.yml/badge.svg)

# Todos
- Disallow overlapping worklogs
- "Working" overlay on DailyRegistrations
- Label issues as "recent" or "search result" in registration dialog list
- Scroll to start-of-work-day automatically, when rendering a new date.
- Double clicking an issue should accept the registration dialog
- Bug: Weekly and monthly totals look at all worklogs
    not just those of current user
- Bug: When adding the first registration for an issue, it cannot be located
    in the list of cached issues, so nothing gets drawn until data for date
    is reloaded from the server
- Bug: Monthly total doesn't look correct at all times
- Bug: Restart required to load profile image and stuff
- Optional: Keyboard shortcuts
- Optional: Resize worklogs
- Optional: Rebrand registrations as worklogs