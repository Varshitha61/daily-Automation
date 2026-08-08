import sqlite3
import datetime

conn = sqlite3.connect('daily_bot.db')
today_str = datetime.date.today().isoformat()
conn.execute("DELETE FROM solved_problems WHERE DATE(solved_at) = ?", (today_str,))
conn.commit()
print("Deleted today's entries")
