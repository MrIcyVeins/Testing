document.addEventListener("DOMContentLoaded", function() {
    const burgerMenu = document.querySelector(".burger-menu");
    const navbarNav = document.querySelector(".navbar-nav");
  
    burgerMenu.addEventListener("click", function() {
      navbarNav.classList.toggle("active");
    });
  });
  