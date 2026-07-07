# Daily Coding Challenge Bot 🤖

## What This Bot Does

This bot automatically fetches a daily coding problem from LeetCode, Codeforces,
CodeChef, and HackerRank every morning, solves each problem using Claude AI, saves
the solution to an organised local folder, and sends you a Telegram notification with
the solution code — all with zero human interaction after first-time setup.

---

## One-Time Setup

**1. Clone the repository**
```bash
git clone https://github.com/your-username/daily-coding-bot.git
cd daily-coding-bot
```

**2. Install Python dependencies**
```bash
pip install -r requirements.txt
```

**3. Install the Playwright browser engine**
```bash
playwright install chromium
```

**4. Create your `.env` file from the template**
```bash
cp .env.example .env
```
Open `.env` in any text editor and fill in every value (see Section 3 below).

**5. How to get your LeetCode session cookie**
   1. Open [leetcode.com](https://leetcode.com) in Chrome or Firefox and log in.
   2. Right-click anywhere → **Inspect** → go to the **Application** tab.
   3. In the left sidebar: **Cookies** → click `https://leetcode.com`.
   4. Find the cookie named **`LEETCODE_SESSION`** — copy its **Value** into `.env`.
   5. Find the cookie named **`csrftoken`** — copy its **Value** into `.env` as `LEETCODE_CSRF_TOKEN`.

**6. How to create a Telegram bot and get your token**
   1. Open Telegram and search for **@BotFather**.
   2. Send `/newbot` and follow the prompts to name your bot.
   3. BotFather will give you a token like `123456789:ABC-xyz...` — copy it to `TELEGRAM_BOT_TOKEN`.
   4. To get your **Chat ID**: message [@userinfobot](https://t.me/userinfobot) on Telegram.
      It will reply with your numeric ID — copy it to `TELEGRAM_CHAT_ID`.
   5. Send `/start` to your new bot before the first run so it can message you.

**7. Start the bot**
```bash
python scheduler.py
```
The bot will print the next scheduled run time and stay alive in the background.
To run immediately (once) without the scheduler:
```bash
python main.py
```

---

## Credential Setup — Platform by Platform

### 🟡 LeetCode
| Key | How to get it |
|-----|---------------|
| `LEETCODE_SESSION` | Browser cookie (see Step 5 above) |
| `LEETCODE_CSRF_TOKEN` | `csrftoken` browser cookie on leetcode.com |

> **Note:** LeetCode session cookies expire after ~2 weeks. Refresh them by logging in and repeating Step 5.

---

### 🔵 Codeforces
| Key | How to get it |
|-----|---------------|
| `CODEFORCES_API_KEY` | [codeforces.com/settings/api](https://codeforces.com/settings/api) → Generate |
| `CODEFORCES_API_SECRET` | Generated alongside the API key (keep it secret) |
| `CODEFORCES_HANDLE` | Your Codeforces username (e.g. `tourist`) |

> The API key/secret pair is used to sign requests. Never share your secret.

---

### 🟢 CodeChef
| Key | How to get it |
|-----|---------------|
| `CODECHEF_USERNAME` | Your CodeChef username or registered email |
| `CODECHEF_PASSWORD` | Your CodeChef account password |

> The bot logs in using Playwright and saves the session cookie to `logs/codechef_session.json`
> so it only needs to re-authenticate when the session expires.

---

### 🟣 HackerRank
| Key | How to get it |
|-----|---------------|
| `HACKERRANK_USERNAME` | Your HackerRank username or email |
| `HACKERRANK_PASSWORD` | Your HackerRank account password |

> Similarly, the session is persisted in `logs/hackerrank_session.json`.

---

### 🤖 Claude AI (Anthropic)
| Key | How to get it |
|-----|---------------|
| `ANTHROPIC_API_KEY` | [console.anthropic.com/account/keys](https://console.anthropic.com/account/keys) → Create key |

---

### 📬 Telegram
| Key | How to get it |
|-----|---------------|
| `TELEGRAM_BOT_TOKEN` | @BotFather → `/newbot` (see Step 6 above) |
| `TELEGRAM_CHAT_ID` | Message @userinfobot on Telegram |

---

## Folder Structure

```
daily-coding-bot/
├── main.py              # Orchestrator — fetches, solves, saves, notifies
├── config.py            # Centralised config + validation from .env
├── scheduler.py         # APScheduler daily cron — run this to start the bot
│
├── platforms/           # One fetcher module per coding platform
│   ├── __init__.py
│   ├── leetcode.py      # GraphQL API + cookie auth
│   ├── codeforces.py    # REST API + Playwright scraper
│   ├── codechef.py      # Playwright login + scraper (no public API)
│   └── hackerrank.py    # Playwright login + scraper (no public API)
│
├── solver/
│   ├── __init__.py
│   └── claude_solver.py # Sends problem to Claude claude-sonnet-4-6, returns raw code
│
├── notifier/
│   ├── __init__.py
│   └── telegram.py      # Telegram Bot API via raw HTTP (no SDK)
│
├── storage/
│   ├── __init__.py
│   └── db.py            # SQLite persistence — tracks every solved problem
│
├── solutions/           # Auto-created solution files (one per day per platform)
│   ├── leetcode/        # solutions/leetcode/2024-01-15_two_sum.py
│   ├── codeforces/
│   ├── codechef/
│   └── hackerrank/
│
├── logs/
│   ├── bot.log                  # Full structured log of every run
│   ├── codechef_session.json    # Persisted CodeChef browser cookies
│   └── hackerrank_session.json  # Persisted HackerRank browser cookies
│
├── daily_bot.db         # SQLite database (auto-created on first run)
├── .env                 # Your credentials (never commit this)
├── .env.example         # Template — commit this instead
└── requirements.txt     # Pinned Python dependencies
```

---

## Environment Variables Reference

| Variable | Required | Default | Description |
|----------|----------|---------|-------------|
| `ANTHROPIC_API_KEY` | ✅ | — | Claude AI API key |
| `TELEGRAM_BOT_TOKEN` | ✅ | — | Telegram bot token from @BotFather |
| `TELEGRAM_CHAT_ID` | ✅ | — | Your Telegram numeric chat ID |
| `LEETCODE_SESSION` | ✅ | — | LeetCode session browser cookie |
| `LEETCODE_CSRF_TOKEN` | ✅ | — | LeetCode CSRF token browser cookie |
| `CODEFORCES_API_KEY` | ✅ | — | Codeforces API key |
| `CODEFORCES_API_SECRET` | ✅ | — | Codeforces API secret |
| `CODEFORCES_HANDLE` | ✅ | — | Your Codeforces username |
| `CODECHEF_USERNAME` | ✅ | — | CodeChef username/email |
| `CODECHEF_PASSWORD` | ✅ | — | CodeChef password |
| `HACKERRANK_USERNAME` | ✅ | — | HackerRank username/email |
| `HACKERRANK_PASSWORD` | ✅ | — | HackerRank password |
| `DAILY_RUN_TIME` | ❌ | `08:00` | Daily execution time (24-hour HH:MM) |
| `SOLUTION_LANGUAGE` | ❌ | `python3` | Solution language: python3, cpp, java |
| `LOG_LEVEL` | ❌ | `INFO` | Logging verbosity: DEBUG, INFO, WARNING, ERROR |

---

## Troubleshooting

**Bot sends no Telegram messages**
→ Make sure you sent `/start` to your bot before the first run.  Bots cannot initiate conversations — you must message them first.

**LeetCode fetch fails with 403**
→ Your session cookie has expired.  Log in to leetcode.com and copy fresh cookie values to `.env`.

**CodeChef/HackerRank login fails**
→ Delete the stale session file (`logs/codechef_session.json` or `logs/hackerrank_session.json`) and let the bot log in fresh.

**Claude returns empty solution**
→ Check your `ANTHROPIC_API_KEY` and ensure your account has available API credits at [console.anthropic.com](https://console.anthropic.com).
