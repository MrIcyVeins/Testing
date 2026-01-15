// server.js — aplicația "Ce pot găti ACUM?"
const express = require("express");
const path = require("path");
const db = require("./db");

const app = express();
const PORT = 3000;

// ====== Config Express ======
app.set("view engine", "ejs");
app.set("views", path.join(__dirname, "views"));

app.use(express.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, "public")));

// ====== Helper: normalizare text ======
function normalizeaza(text) {
  return (text || "")
    .trim()
    .toLowerCase()
    .replace(/\s+/g, " ");
}

// Parsează inputul: virgulă / newline / punct și virgulă
function extrageIngrediente(input) {
  return (input || "")
    .split(/[,;\n]/g)
    .map((x) => normalizeaza(x))
    .filter(Boolean);
}

// Citește ingredientele din DB
function citesteCamara() {
  return new Promise((resolve, reject) => {
    db.all(`SELECT nume FROM camara ORDER BY nume ASC`, [], (err, rows) => {
      if (err) return reject(err);
      resolve(rows.map((r) => r.nume));
    });
  });
}

// Înlocuiește complet cămara (MVP rapid)
function inlocuiesteCamara(lista) {
  return new Promise((resolve, reject) => {
    db.serialize(() => {
      db.run(`DELETE FROM camara`, [], (err) => {
        if (err) return reject(err);

        const stmt = db.prepare(`INSERT INTO camara(nume) VALUES (?)`);
        for (const nume of lista) stmt.run([nume]);
        stmt.finalize((err2) => {
          if (err2) return reject(err2);
          resolve();
        });
      });
    });
  });
}

// ====== TheMealDB ======
const BAZA_API = "https://www.themealdb.com/api/json/v1/1";

// Rețete după ingredient (id, titlu, imagine)
async function reteteDupaIngredient(ingredient) {
  const url = `${BAZA_API}/filter.php?i=${encodeURIComponent(ingredient)}`;
  const r = await fetch(url);
  if (!r.ok) return [];
  const data = await r.json();
  return data.meals || [];
}

// Detalii rețetă (ingrediente + instrucțiuni)
async function detaliiReteta(idMeal) {
  const url = `${BAZA_API}/lookup.php?i=${encodeURIComponent(idMeal)}`;
  const r = await fetch(url);
  if (!r.ok) return null;
  const data = await r.json();
  return (data.meals && data.meals[0]) || null;
}

// Extrage ingredientele din detalii (strIngredient1..20)
function extrageIngredienteDinDetalii(det) {
  const ingrediente = [];
  for (let i = 1; i <= 20; i++) {
    const nume = det[`strIngredient${i}`];
    if (nume && normalizeaza(nume)) ingrediente.push(normalizeaza(nume));
  }
  return ingrediente;
}

// Sugestii: combină rețetele pentru fiecare ingredient și calculează scor
async function calculeazaSugestii(ingredienteUtilizator) {
  const map = new Map(); // idMeal -> {idMeal, titlu, imagine, scor, potriviri:Set}

  for (const ing of ingredienteUtilizator) {
    const rezultate = await reteteDupaIngredient(ing);

    for (const m of rezultate) {
      const id = m.idMeal;
      if (!map.has(id)) {
        map.set(id, {
          idMeal: id,
          titlu: m.strMeal,
          imagine: m.strMealThumb,
          scor: 0,
          potriviri: new Set()
        });
      }

      const entry = map.get(id);
      if (!entry.potriviri.has(ing)) {
        entry.potriviri.add(ing);
        entry.scor += 1;
      }
    }
  }

  return [...map.values()]
    .sort((a, b) => (b.scor - a.scor) || a.titlu.localeCompare(b.titlu))
    .slice(0, 30);
}

// ====== Rute ======
app.get("/", async (req, res) => {
  try {
    const ingredienteCamara = await citesteCamara();
    const sugestii = ingredienteCamara.length
      ? await calculeazaSugestii(ingredienteCamara)
      : [];

    res.render("index", {
      ingredienteCamara,
      sugestii,
      mesaj: ingredienteCamara.length
        ? ""
        : "Adaugă ingrediente și apasă „Caută” ca să vezi rețete."
    });
  } catch {
    res.status(500).send("Eroare la citire DB / API.");
  }
});

app.post("/cauta", async (req, res) => {
  try {
    const lista = extrageIngrediente(req.body.ingrediente);
    await inlocuiesteCamara(lista);
    res.redirect("/");
  } catch {
    res.status(500).send("Eroare la salvare ingrediente.");
  }
});

// La click pe rețetă: returnează lipsurile (afișate cu roșu în UI)
app.get("/api/lipsuri/:idMeal", async (req, res) => {
  try {
    const ingredienteCamara = await citesteCamara();
    const setCamara = new Set(ingredienteCamara.map(normalizeaza));

    const det = await detaliiReteta(req.params.idMeal);
    if (!det) return res.json({ ok: false, lipsuri: [] });

    const toateIngrediente = extrageIngredienteDinDetalii(det);
    const lipsuri = toateIngrediente.filter((ing) => !setCamara.has(ing));

    res.json({ ok: true, lipsuri });
  } catch {
    res.json({ ok: false, lipsuri: [] });
  }
});

app.listen(PORT, () => console.log(`Rulează: http://localhost:${PORT}`));
