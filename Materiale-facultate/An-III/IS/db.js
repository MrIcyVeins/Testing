// db.js — configurare SQLite (bază de date locală)
const sqlite3 = require("sqlite3").verbose();
const path = require("path");

const db = new sqlite3.Database(path.join(__dirname, "pantry.db"));

db.serialize(() => {
  // Tabel simplu: ingredientele pe care le ai acum
  db.run(`
    CREATE TABLE IF NOT EXISTS camara (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      nume TEXT NOT NULL
    )
  `);

  db.run(`CREATE INDEX IF NOT EXISTS idx_camara_nume ON camara(nume)`);
});

module.exports = db;
