from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    # return a template to display the stock information

    transactions = db.execute("SELECT stocks.stock_name, stocks.stock_symbol, SUM(transactions.shares) AS shares, SUM(transactions.shares * transactions.transaction_price) AS principle FROM transactions INNER JOIN stocks ON transactions.stock_id=stocks.id WHERE transactions.user_id=:user_id GROUP BY transactions.stock_id", user_id=session["user_id"])

    for stock in transactions:
        current_stock = lookup(stock["stock_symbol"])
        stock["current_price"] = current_stock["price"]

    cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])

    cash = cash[0]["cash"]

    grand_total = cash

    for stock in transactions:
        grand_total += round((stock["shares"] * stock["current_price"]), 2)

    return render_template("portfolio.html", stocks=transactions, cash=cash, grand_total=grand_total)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    if request.method == "POST":
        # ensure stock input was not blank
        if not request.form.get("stock"):
            return apology("Must input Stock Symbol!")

        # ensure the quantity value is an integer
        try:
            int(request.form.get("quantity"))
        except:
            return apology("Quantity must be a Integer!")

        # ensure quantity is a positive number
        if int(request.form.get("quantity")) < 1:
            return apology("Must be a positive number!")

        # lookup stock information from Yahoo (helper method)
        stock = lookup(request.form.get("stock"))

        # ensure that the input is a valid stock symbol
        if not stock:
            return apology("Not a valid stock symbol!")

        cost_of_stock = stock["price"] * float(request.form.get("quantity"))

        user_cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

        if not cost_of_stock <= user_cash[0]["cash"]:
            return apology("Not enough cash to purchase stock!")

        stock_exists = db.execute("SELECT * FROM stocks WHERE stock_symbol=:stock_symbol", stock_symbol=stock["symbol"])

        if not stock_exists:
            stock_exists = db.execute("INSERT INTO stocks (stock_name, stock_symbol) VALUES (:stock_name, :stock_symbol)", stock_name=stock["name"], stock_symbol=stock["symbol"])
        else:
            stock_exists = stock_exists[0]["id"]

        db.execute("INSERT INTO transactions (action, transaction_price, shares, stock_id, user_id) VALUES (:action, :transaction_price, :shares, :stock_id, :user_id)", action=0, transaction_price=round(stock["price"], 2), shares=request.form.get("quantity"), stock_id=stock_exists, user_id=session["user_id"])

        db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=round((user_cash[0]["cash"] - cost_of_stock), 2), id=session["user_id"])

        # redirect user to home page
        return redirect(url_for("index"))

    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""

    transactions = db.execute("SELECT * FROM transactions INNER JOIN stocks ON transactions.stock_id=stocks.id WHERE user_id=:user_id", user_id=session["user_id"])

    return render_template("transactions.html", stocks=transactions)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        # ensure stock input was not blank
        if not request.form.get("stock"):
            return apology("Must input Stock Symbol!")

        # lookup stock information from Yahoo (helper method)
        stock = lookup(request.form.get("stock"))

        # ensure that the input is a valid stock symbol
        if not stock:
            return apology("Not a valid stock symbol!")

        # return a template to display the stock information
        return render_template("quoted.html", stock=stock)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("Must have username!")

        # ensure username is unique
        unique_user = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        if unique_user:
            return apology("Username must be unique!")

        # ensure password was submitted
        if not request.form.get("password"):
            return apology("Must provide password!")

        # ensure password matches password confirmation
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords don't match!")
        else:
            # hash password
            hashed_password = pwd_context.encrypt(request.form.get("password"))

        # Insert user into the database
        new_user = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=hashed_password)

        # remember which user has logged in
        # INSERT INTO returns the id of the user to the variable new_user
        session["user_id"] = new_user

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""

    if request.method == "POST":
        # ensure stock input was not blank
        if not request.form.get("stock"):
            return apology("Must input Stock Symbol!")

        # ensure the quantity value is an integer
        try:
            int(request.form.get("quantity"))
        except:
            return apology("Quantity must be a Integer!")

        # ensure a positive quantity is entered
        if int(request.form.get("quantity")) < 1:
            return apology("Must be a positive number!")

        # lookup stock information from Yahoo (helper method)
        stock = lookup(request.form.get("stock"))

        # ensure that the input is a valid stock symbol
        if not stock:
            return apology("Not a valid stock symbol!")

        user_has_stock = db.execute("SELECT stocks.id, SUM(transactions.shares) AS shares FROM transactions INNER JOIN stocks ON transactions.stock_id=stocks.id WHERE transactions.user_id=:user_id AND stocks.stock_symbol=:stock_symbol GROUP BY stocks.stock_symbol", user_id=session["user_id"], stock_symbol=stock["symbol"])

        if not user_has_stock:
            return apology("You Do Not Have Any Shares!")

        if user_has_stock[0]["shares"] < int(request.form.get("quantity")):
            return apology("You Do Not Have That Many Shares!")

        cost_of_stock = stock["price"] * float(request.form.get("quantity"))

        user_cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

        shares_sold = -1 * int(request.form.get("quantity"))

        print("{}".format(shares_sold))

        db.execute("INSERT INTO transactions (action, transaction_price, shares, stock_id, user_id) VALUES (:action, :transaction_price, :shares, :stock_id, :user_id)", action=1, transaction_price=round(stock["price"], 2), shares=shares_sold, stock_id=user_has_stock[0]["id"], user_id=session["user_id"])

        db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=round((user_cash[0]["cash"] + cost_of_stock), 2), id=session["user_id"])

        # redirect user to home page
        return redirect(url_for("index"))

    else:
        return render_template("sell.html")

@app.route("/password", methods=["GET", "POST"])
def password():
    """Update user password."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        if not request.form.get("old_password"):
            return apology("Must provide Old Password!")

        # ensure password was submitted
        if not request.form.get("new_password"):
            return apology("Must provide password!")

        # ensure password matches password confirmation
        if request.form.get("new_password") != request.form.get("confirmation"):
            return apology("Passwords don't match!")

        # ensure old password is correctly entered
        rows = db.execute("SELECT * FROM users WHERE id=:id", id=session["user_id"])

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("old_password"), rows[0]["hash"]):
            return apology("Invalid username and/or password!")

        # Create password hash for new password
        new_password = pwd_context.encrypt(request.form.get("new_password"))

        # Update password hash in the database
        db.execute("UPDATE users SET hash=:hash WHERE id=:id", hash=new_password, id=session["user_id"])

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("password.html")
