# ==============================================================
# GitHub Actions Workflow Trigger Script
# Triggers the "Daily Coding Bot Run" workflow via GitHub API
# Schedule: 9:30 AM IST daily (via Windows Task Scheduler)
# ==============================================================

# ── Configuration ─────────────────────────────────────────────
$GITHUB_TOKEN  = $env:GITHUB_PAT          # Read from environment variable (set in Task Scheduler)
$REPO_OWNER    = "Varshitha61"
$REPO_NAME     = "daily-Automation"
$WORKFLOW_FILE = "daily-run.yml"
$BRANCH        = "main"

# ── Log Setup ─────────────────────────────────────────────────
$LogDir  = "$PSScriptRoot\logs"
$LogFile = "$LogDir\trigger_$(Get-Date -Format 'yyyy-MM-dd').log"
if (-not (Test-Path $LogDir)) { New-Item -ItemType Directory -Path $LogDir | Out-Null }

function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $line = "[$timestamp] [$Level] $Message"
    Write-Host $line
    Add-Content -Path $LogFile -Value $line
}

# ── Validate Token ────────────────────────────────────────────
if (-not $GITHUB_TOKEN) {
    Write-Log "GITHUB_PAT environment variable is not set. Cannot trigger workflow." "ERROR"
    exit 1
}

# ── Trigger Workflow via GitHub REST API ──────────────────────
$ApiUrl = "https://api.github.com/repos/$REPO_OWNER/$REPO_NAME/actions/workflows/$WORKFLOW_FILE/dispatches"
$Headers = @{
    "Authorization" = "Bearer $GITHUB_TOKEN"
    "Accept"        = "application/vnd.github+json"
    "X-GitHub-Api-Version" = "2022-11-28"
}
$Body = @{ ref = $BRANCH } | ConvertTo-Json

Write-Log "Triggering workflow '$WORKFLOW_FILE' on branch '$BRANCH' for $REPO_OWNER/$REPO_NAME ..."

try {
    $Response = Invoke-RestMethod -Uri $ApiUrl -Method POST -Headers $Headers -Body $Body -ContentType "application/json"
    Write-Log "Workflow triggered successfully! GitHub API responded with 204 (no content = success)." "SUCCESS"
} catch {
    $StatusCode = $_.Exception.Response.StatusCode.value__
    $ErrorBody  = $_.ErrorDetails.Message

    if ($StatusCode -eq 204) {
        # 204 No Content = success (GitHub returns no body on success)
        Write-Log "Workflow triggered successfully (HTTP 204)." "SUCCESS"
    } elseif ($StatusCode -eq 401) {
        Write-Log "Authentication failed (HTTP 401). Your GITHUB_PAT token may be expired or invalid." "ERROR"
        Write-Log "Error: $ErrorBody" "ERROR"
        exit 1
    } elseif ($StatusCode -eq 404) {
        Write-Log "Workflow not found (HTTP 404). Check REPO_OWNER, REPO_NAME, and WORKFLOW_FILE." "ERROR"
        Write-Log "Error: $ErrorBody" "ERROR"
        exit 1
    } elseif ($StatusCode -eq 422) {
        Write-Log "Unprocessable request (HTTP 422). The branch '$BRANCH' may not exist." "ERROR"
        Write-Log "Error: $ErrorBody" "ERROR"
        exit 1
    } else {
        Write-Log "Unexpected error (HTTP $StatusCode): $ErrorBody" "ERROR"
        exit 1
    }
}

Write-Log "Done. Check GitHub Actions at: https://github.com/$REPO_OWNER/$REPO_NAME/actions"
