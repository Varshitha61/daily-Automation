# ==============================================================
# Windows Task Scheduler Setup Script
# Run this ONCE as Administrator to register the daily trigger
# ==============================================================
# USAGE:
#   1. Create a GitHub PAT at: https://github.com/settings/tokens
#      → Give it "repo" scope (Actions: write permission)
#   2. Replace YOUR_TOKEN_HERE below with your actual PAT
#   3. Run this script as Administrator in PowerShell
# ==============================================================

param(
    [Parameter(Mandatory=$true)]
    [string]$GitHubPAT
)

# ── Settings ──────────────────────────────────────────────────
$TaskName    = "DailyCodingBot-GitHubTrigger"
$ScriptPath  = "C:\Users\varsh\Desktop\projects\daily ai\daily-coding-bot\trigger_github_workflow.ps1"
$RunTime     = "09:30"   # 9:30 AM IST daily

# ── Validate script exists ────────────────────────────────────
if (-not (Test-Path $ScriptPath)) {
    Write-Error "Trigger script not found at: $ScriptPath"
    exit 1
}

# ── Build the Task Action ─────────────────────────────────────
# Uses powershell.exe so the GITHUB_PAT env var is passed inline
$Action = New-ScheduledTaskAction `
    -Execute "powershell.exe" `
    -Argument "-NonInteractive -NoProfile -ExecutionPolicy Bypass -Command `"& {`$env:GITHUB_PAT='$GitHubPAT'; & '$ScriptPath'}`""

# ── Build the Trigger (daily at 9:30 AM) ─────────────────────
$Trigger = New-ScheduledTaskTrigger -Daily -At $RunTime

# ── Build the Settings ───────────────────────────────────────
$Settings = New-ScheduledTaskSettingsSet `
    -ExecutionTimeLimit (New-TimeSpan -Minutes 5) `
    -RunOnlyIfNetworkAvailable `
    -StartWhenAvailable `   # Will run even if PC was off at the scheduled time
    -WakeToRun:$false

# ── Register (or update) the task ────────────────────────────
$ExistingTask = Get-ScheduledTask -TaskName $TaskName -ErrorAction SilentlyContinue

if ($ExistingTask) {
    Write-Host "Task '$TaskName' already exists — updating it..." -ForegroundColor Yellow
    Set-ScheduledTask -TaskName $TaskName -Action $Action -Trigger $Trigger -Settings $Settings | Out-Null
    Write-Host "Task updated successfully." -ForegroundColor Green
} else {
    Register-ScheduledTask `
        -TaskName $TaskName `
        -Action $Action `
        -Trigger $Trigger `
        -Settings $Settings `
        -RunLevel Highest `
        -Description "Triggers the Daily Coding Bot GitHub Actions workflow at 9:30 AM IST every day." | Out-Null
    Write-Host "Task '$TaskName' registered successfully." -ForegroundColor Green
}

Write-Host ""
Write-Host "✅ Setup complete!" -ForegroundColor Cyan
Write-Host "   Task name  : $TaskName"
Write-Host "   Runs daily : $RunTime IST"
Write-Host "   Script     : $ScriptPath"
Write-Host ""
Write-Host "To test it now, run:" -ForegroundColor Yellow
Write-Host "   Start-ScheduledTask -TaskName '$TaskName'"
Write-Host ""
Write-Host "To view logs, check: $PSScriptRoot\daily-coding-bot\logs\" -ForegroundColor Yellow
