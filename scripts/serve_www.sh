#!/usr/bin/env bash
# Simple static server for the generated site under /www
# Defaults: host 127.0.0.1, port 8080, root = /www
# Usage:
#   scripts/serve_www.sh                # serves /www
#   scripts/serve_www.sh --repo-root    # serves repository root (then open /www/)
#   scripts/serve_www.sh --port 3000 --host 0.0.0.0

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SERVE_DIR="$ROOT/www"
HOST="127.0.0.1"
PORT="8080"
OPEN="1"
OPEN_PATH="/"
POLISHED="0"

while [[ $# -gt 0 ]]; do
  case "$1" in
    --repo-root)
      SERVE_DIR="$ROOT"
      OPEN_PATH="/www/"
      shift
      ;;
    --www-root|--www)
      SERVE_DIR="$ROOT/www"
      OPEN_PATH="/"
      shift
      ;;
    --host)
      HOST="${2:-}"; shift 2 ;;
    --port|-p)
      PORT="${2:-}"; shift 2 ;;
    --no-open)
      OPEN="0"; shift ;;
    --polished)
      POLISHED="1"; shift ;;
    --help|-h)
      echo "Usage: $0 [--repo-root|--www-root] [--host HOST] [--port PORT] [--no-open] [--polished]"; exit 0 ;;
    *) echo "Unknown option: $1"; exit 1 ;;
  esac
done

# Generate site if serving www; build polished if requested
if [[ "$SERVE_DIR" == "$ROOT/www" ]]; then
  if [[ "$POLISHED" == "1" ]]; then
    echo "[serve_www] Generating /www (polished) ..."
    SITE_POLISHED=1 python3 "$ROOT/scripts/generate_www.py" --polished || true
  elif [[ ! -f "$ROOT/www/index.html" ]]; then
    echo "[serve_www] Generating /www ..."
    python3 "$ROOT/scripts/generate_www.py" || true
  fi
fi

URL="http://$HOST:$PORT$OPEN_PATH"

open_browser() {
  if command -v xdg-open >/dev/null 2>&1; then
    xdg-open "$URL" >/dev/null 2>&1 || true
  elif command -v open >/dev/null 2>&1; then
    open "$URL" >/dev/null 2>&1 || true
  fi
}

echo "[serve_www] Serving: $SERVE_DIR"
echo "[serve_www] URL:     $URL"

if command -v npx >/dev/null 2>&1; then
  # Start server in background so we can open browser, then wait
  npx --yes http-server "$SERVE_DIR" -p "$PORT" -a "$HOST" -c-1 &
  PID=$!
  trap 'kill "$PID" 2>/dev/null || true' EXIT
  sleep 1
  if [[ "$OPEN" == "1" ]]; then open_browser; fi
  wait "$PID"
else
  echo "[serve_www] npx not found; falling back to Python http.server"
  ( cd "$SERVE_DIR" && python3 -m http.server "$PORT" --bind "$HOST" ) &
  PID=$!
  trap 'kill "$PID" 2>/dev/null || true' EXIT
  sleep 1
  if [[ "$OPEN" == "1" ]]; then open_browser; fi
  wait "$PID"
fi
