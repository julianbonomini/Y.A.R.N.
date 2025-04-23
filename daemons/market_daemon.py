from flask import Flask, request, jsonify
import yfinance as yf
import argparse
import logging

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
}


@app.route("/cached-price/<symbol>")
def get_price(symbol):
    symbol = symbol.upper()
    price = stock_data_cache.get(symbol)
    if price is None:
        return jsonify({"error": f"Price for symbol '{symbol}' not found"}), 404
    return jsonify({"symbol": symbol, "price": price})

@app.route("/quotes")
def refresh_and_return_all_quotes():
    log.info("Fetching all configured symbols")
    for symbol in settings["symbols"]:
        symbol = symbol.upper()
        try:
            data = yf.Ticker(symbol).info
            stock_data_cache[symbol] = data["regularMarketPrice"]
        except Exception as e:
            log.error(f"Error fetching {symbol}: {e}")
    return jsonify(stock_data_cache)

@app.route("/config", methods=["POST"])
def update_config():
    json = request.get_json()
    if "symbols" in json:
        settings["symbols"] = [s.upper() for s in json["symbols"]]
    return jsonify({"status": "ok", "settings": settings})

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Stock Quote Daemon")
    parser.add_argument(
        "--symbols", nargs="+", default=[],
        help="Initial list of stock symbols (space-separated)"
    )
    args = parser.parse_args()

    settings["symbols"] = [s.upper() for s in args.symbols]

    print(f"[MARKET_DAEMON]: Starting with symbols={settings['symbols']}")

    app.run(host="0.0.0.0", port=8080)
