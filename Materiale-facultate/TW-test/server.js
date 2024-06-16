// imports
const express = require("express");
const app = express();
const path = require("path");
const fs = require("fs");
const sass = require("sass"); // pachetul sass

// application runtime variables
const port = 8080;

// other variables
// E4 P13
const obGlobal = {
  obErori: null,
  folderScss: path.join(__dirname, "Proiect/resurse/scss"),
  folderCss: path.join(__dirname, "Proiect/resurse/css")
};

// E5 galerie-statica
const sharp = require('sharp');

const resizeImage = (imagePath, width, outputPath) => {
  sharp(imagePath)
      .resize(width)
      .toFile(outputPath, (err) => {
          if (err) {
              console.error('Error resizing image:', err);
          }
      });
};

// Call this function when your server starts or when you add new images
const generateResizedImages = () => {
  const data = JSON.parse(fs.readFileSync(path.join(__dirname, "Proiect/data/images.json")));
  data.imagini.forEach(image => {
      const imagePath = path.join(__dirname, "Proiect", "resurse", "imagini", image.cale_imagine);
      const smallImagePath = path.join(__dirname, "Proiect", "resurse", "imagini", 'small_' + image.cale_imagine);
      const mediumImagePath = path.join(__dirname, "Proiect", "resurse", "imagini", 'medium_' + image.cale_imagine);

      resizeImage(imagePath, 300, smallImagePath);
      resizeImage(imagePath, 600, mediumImagePath);
  });
};


// E5 - Compilare automata scss
// Compile SCSS function
const compileazaScss = (caleScss, caleCss) => {
  if (!path.isAbsolute(caleScss)) {
    caleScss = path.join(obGlobal.folderScss, caleScss);
  }
  if (!path.isAbsolute(caleCss)) {
    caleCss = path.join(obGlobal.folderCss, caleCss);
  }
  
  const cssFolder = path.dirname(caleCss);
  if (!fs.existsSync(cssFolder)) {
    fs.mkdirSync(cssFolder, { recursive: true });
  }

  const backupFolder = path.join(__dirname, "Proiect/backup", path.relative(__dirname, cssFolder));
  if (!fs.existsSync(backupFolder)) {
    fs.mkdirSync(backupFolder, { recursive: true });
  }

  const cssFileName = path.basename(caleCss);
  const backupCssFile = path.join(backupFolder, cssFileName);
  
  if (fs.existsSync(caleCss)) {
    fs.copyFileSync(caleCss, backupCssFile);
  }
  
  try {
    const result = sass.renderSync({ file: caleScss });
    fs.writeFileSync(caleCss, result.css);
    console.log(`Compiled ${caleScss} to ${caleCss}`);
  } catch (err) {
    console.error(`Error compiling ${caleScss}:`, err);
  }
};

compileazaScss('nav.scss', 'nav.css');
compileazaScss('custom.scss', 'custom.css');

// Compilează toate fișierele SCSS la pornirea serverului
const compileAllScss = () => {
  const scssFiles = fs.readdirSync(obGlobal.folderScss);
  scssFiles.forEach(file => {
    if (path.extname(file) === '.scss') {
      compileazaScss(file, path.basename(file, '.scss') + '.css');
    }
  });
};

// Asigură-te că SCSS-urile sunt recompilate la modificare
fs.watch(obGlobal.folderScss, (eventType, filename) => {
  if (path.extname(filename) === '.scss') {
    compileazaScss(filename, path.basename(filename, '.scss') + '.css');
  }
});

// Initial SCSS compilation
// const compileAllScss = () => {
//   const scssFiles = fs.readdirSync(obGlobal.folderScss);
//   scssFiles.forEach(file => {
//     if (path.extname(file) === ".scss") {
//       compileazaScss(file, path.basename(file, ".scss") + ".css");
//     }
//   });
// };

// Compile SCSS files initially
// compileAllScss();


// Watch SCSS folder for changes
// fs.watch(obGlobal.folderScss, (eventType, filename) => {
//   if (filename && path.extname(filename) === ".scss") {
//     compileazaScss(filename, path.basename(filename, ".scss") + ".css");
//   }
// });

// Generate resized images when the server starts
generateResizedImages();


// setup stuff
app.set("view engine", "ejs");
app.set("trust proxy", true);

app.use(express.static(__dirname + "/Proiect"));
app.set("views", path.join(__dirname, "/Proiect/views"));

// Helper function to get current quarter of the hour
const getQuarter = (date) => {
  const minutes = date.getMinutes();
  if (minutes < 15) return 1;
  if (minutes < 30) return 2;
  if (minutes < 45) return 3;
  return 4;
};

// Read image data from JSON file
// const getImageData = () => {
//   const data = JSON.parse(fs.readFileSync(path.join(__dirname, "Proiect/data/images.json")));
//   const currentQuarter = getQuarter(new Date());
//   const filteredImages = data.imagini.filter(img => img.sfert_ora == currentQuarter).slice(0, 10);
//   return { images: filteredImages, galleryPath: data.cale_galerie };
// };

// Read image data from JSON file
const getImageData = (customDate = null) => {
  const data = JSON.parse(fs.readFileSync(path.join(__dirname, "Proiect/data/images.json")));
  const date = customDate || new Date();
  const currentQuarter = getQuarter(date);
  const filteredImages = data.imagini.filter(img => img.sfert_ora == currentQuarter).slice(0, 10);
  console.log(`Date: ${date}, Current Quarter: ${currentQuarter}, Images: ${filteredImages.map(img => img.titlu).join(', ')}`);
  return { images: filteredImages, galleryPath: data.cale_galerie };
};


// app.get("/", (req, res) => {
//   res.render(path.join(__dirname, "/Proiect/index.ejs"));
// });

// app.get("/index", (req, res) => {
//   res.render(path.join(__dirname, "/Proiect/index.ejs"));
// });
// app.get("/home", (req, res) => {
//   res.render(path.join(__dirname, "/Proiect/index.ejs"));
// });


app.get("/", (req, res) => {
  const { images, galleryPath } = getImageData();
  res.render(path.join(__dirname, "Proiect/index"), { images: images, galleryPath: galleryPath });
});

app.get("/index", (req, res) => {
  const { images, galleryPath } = getImageData();
  res.render(path.join(__dirname, "Proiect/index"), { images: images, galleryPath: galleryPath });
});

app.get("/home", (req, res) => {
  const { images, galleryPath } = getImageData();
  res.render(path.join(__dirname, "Proiect/index"), { images: images, galleryPath: galleryPath });
});

app.get("/about", (req, res) => {
  res.render(path.join(__dirname, "Proiect", "views", "pagini", "about.ejs"), {
    ip: req.ip,
  });
});

app.get("/gallery", (req, res) => {
  const { images, galleryPath } = getImageData();
  res.render("pagini/gallery", { images, galleryPath });
});

// Ruta de testare pentru a schimba ora curentă
app.get("/test-gallery", (req, res) => {
  const { hour, minute } = req.query;
  const testDate = new Date();
  if (hour) testDate.setHours(parseInt(hour));
  if (minute) testDate.setMinutes(parseInt(minute));
  const { images, galleryPath } = getImageData(testDate);
  res.render(path.join(__dirname, "Proiect/views/pagini/gallery"), { images: images, galleryPath: galleryPath });
});


app.get("/gallery", (req, res) => {
  const { images, galleryPath } = getImageData();
  res.render("pagini/gallery", { images, galleryPath });
});
// E4 P17
app.get("/resurse/*", (req, res) => {
  console.log("You are not allowed here WARN");
  res.render(
    path.join(__dirname, "Proiect", "views", "pagini", "403_forbidden.ejs")
  );
});
// E4 P18
app.get("/favicon.ico", (req, res) => {
  res.sendFile(
    path.join(__dirname, "Proiect", "resurse", "favicon", "favicon.ico")
  );
});
// E4 P19
app.get("/*.ejs", (req, res) => {
  res.render(
    path.join(__dirname, "Proiect", "views", "pagini", "400_badRequest.ejs")
  );
});
app.get("/:path", (req, res) => {
  const pathToPageFile = path.join(
    app.get("views"),
    "pagini",
    req.params.path + ".ejs"
  );
  res.render(pathToPageFile, undefined, (err, html) => {
    if (err && err.message.startsWith("Failed to lookup view")) {
      res.render(
        path.join(__dirname, "Proiect", "views", "pagini", "eroare.ejs"),
        {
          title: obGlobal.obErori.eroare_default.titlu,
          text: obGlobal.obErori.eroare_default.text,
          img: path.join(
            obGlobal.obErori.cale_baza,
            obGlobal.obErori.eroare_default.imagine
          ),
        }
      ); // E4 P14
    } else {
      res.send(html);
    }
  });
});

app.listen(port, () => {
  console.log("Server started at http://localhost:" + port);
  console.log("__dirname=" + __dirname);
  console.log("__filename=" + __filename);
  console.log("process.cwd() -> " + process.cwd());
  console.log(app.get("views"));
  console.log(app.get("view engine"));

  generateFolders();
  initErori();
});

//E4 P20
function generateFolders() {
  const vect_foldere = ["temp", "temp1", "backup"];

  vect_foldere.forEach((i) => {
    const folderPath = path.join(__dirname, "Proiect", i);
    if (!fs.existsSync(folderPath)) {
      fs.mkdirSync(folderPath, { recursive: true }, (err) => {
        if (err) {
          return console.error(err);
        }
        console.log("Directory " + folderPath + " created successfully!");
      });
    }
  });
}

//E4 P13
function initErori() {
  obGlobal.obErori = JSON.parse(
    fs.readFileSync(path.join(__dirname, "Proiect", "erori.json")).toString()
  );
}
// E4 P3. Să se afișeze calea folderului în care se găsește fișierul index.js (__dirname), calea fișierului (__filename) și folderul curent de lucru
// (process.cwd()). Sunt __dirname și process.cwd() același lucru întotdeauna?
// process.cwd() indica locul din care a fost lansata comanda node, __dirname indica locatia folderului parinte al fisierului executat