from flask import Flask, request, jsonify
import yfinance as yf
import argparse
import logging
import time

app = Flask(__name__)

log = logging.getLogger('werkzeug')
log.setLevel(logging.INFO)

formatter = logging.Formatter('[MARKET_DAEMON] %(message)s')
handler = logging.StreamHandler()
handler.setFormatter(formatter)
log.handlers.clear()
log.addHandler(handler)

stock_data_cache = {}
settings = {
    "symbols": [],
    "trackers": [],
}

def cacheSymbolData(symbol, typeOfSymbol):
    yf_data = yf.Ticker(symbol).info
    price = yf_data.get("regularMarketPrice")
    open_price = yf_data.get("regularMarketOpen")
    prev_close = yf_data.get("previousClose")
    change_from_open = None
    pct_change_from_open = None
    change_from_prev_close = None
    pct_change_from_prev_close = None
    if price is not None:
        if open_price:
            change_from_open = price - open_price
            pct_change_from_open = (change_from_open / open_price) * 100
        if prev_close:
            change_from_prev_close = price - prev_close
            pct_change_from_prev_close = (change_from_prev_close / prev_close) * 100
    stock_data_cache[symbol] = {
        "price": price,
        "change_from_open": change_from_open,
        "percent_change_from_open": pct_change_from_open,
        "change_from_prev_close": change_from_prev_close,
        "percent_change_from_prev_close": pct_change_from_prev_close,
        "type": typeOfSymbol,
    }

@app.route("/market-status")
def get_market_status():
    try:
        us_market = yf.Market("US")
        return jsonify(us_market.status), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route("/cached-quote/<symbol>")
def get_cached_quote(symbol):
    symbol = symbol.upper()
    data = stock_data_cache.get(symbol)
    if data is None:
        return jsonify({"error": f"Price for symbol '{symbol}' not found"}), 404
    return jsonify(data)

@app.route("/quote/<symbol>")
def get_quote(symbol):
    symbol = symbol.upper()
    try:
        cacheSymbolData(symbol, "stock")
    except Exception as e:
        log.error(f"Error fetching {symbol}: {e}")
    return jsonify(stock_data_cache[symbol])


@app.route("/quotes")
def get_all_quotes():
    log.info("Fetching all configured symbols")
    for symbol in settings["symbols"]:
        symbol = symbol.upper()
        try:
            cacheSymbolData(symbol, "stock")
            time.sleep(0.1) # throttle to avoid limiting
        except Exception as e:
            log.error(f"Error fetching {symbol}: {e}")
    for tracker in settings["trackers"]:
        tracker = tracker.upper()
        try:
            cacheSymbolData(tracker, "index")
            time.sleep(0.25) # throttle to avoid limiting
        except Exception as e:
            log.error(f"Error fetching {symbol}: {e}")
    return jsonify(stock_data_cache)

@app.route("/config", methods=["POST"])
def update_config():
    json = request.get_json()
    if "symbols" in json:
        settings["symbols"] = [s.upper() for s in json["symbols"]]
    if "trackers" in json:
        settings["trackers"] = [s.upper() for s in json["trackers"]]
    return jsonify({"status": "ok", "settings": settings})

@app.route("/ready")
def health():
    return jsonify({"status": "ok"}), 200

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Stock Quote Daemon")
    parser.add_argument(
        "--symbols", nargs="+", default=[],
        help="Initial list of stock symbols (space-separated)"
    )
    parser.add_argument(
        "--trackers", nargs="+", default=[],
        help="Initial list of market trackers (space-separated)"
    )
    args = parser.parse_args()

    settings["symbols"] = [s.upper() for s in args.symbols]
    settings["trackers"] = [s.upper() for s in args.trackers]

    print(f"[MARKET_DAEMON]: Starting with symbols={settings['symbols']}, trackers={settings['trackers']}")

    app.run(host="0.0.0.0", port=8080)
