# 🎬 Bioskop — 2D OpenGL simulacija

**2D Projekat iz predmeta Računarska grafika**  
**Autor:** Ognjen Miletic — SV47/2023

Interaktivna 2D simulacija bioskopa iz ptičje perspektive, izrađena u jeziku **C++** uz **OpenGL 3.3 Core Profile**. Aplikacija obuhvata rezervaciju i kupovinu sedišta, animirani ulazak gledalaca, projekciju filma i njihov izlazak, nakon čega se sala vraća u početno stanje.

## ✨ Funkcionalnosti

- **50 sedišta** raspoređenih u 5 redova po 10 mesta.
- **Rezervacija klikom:** slobodno sedište (plavo) postaje rezervisano (žuto), a ponovni klik otkazuje rezervaciju.
- **Kupovina karata:** tasteri `1`–`9` kupuju odgovarajući broj susednih slobodnih sedišta, pretragom od desne strane poslednjeg reda; kupljena sedišta su crvena.
- **Početak simulacije:** `Enter` otvara vrata, uklanja početno zatamnjenje i zaključava promenu statusa sedišta.
- **Ulazak gledalaca:** nasumičan broj gledalaca dobija jedinstvena rezervisana ili kupljena mesta i kreće se konstantnom brzinom, prvo vertikalno do reda, pa horizontalno do sedišta.
- **Projekcija:** kada svi sednu, vrata se zatvaraju i platno menja nasumične boje tokom najmanje **20 sekundi**.
- **Izlazak i reset:** nakon projekcije vrata se otvaraju, gledaoci izlaze obrnutom putanjom, a potom se zatamnjenje vraća i započinje novi ciklus rezervacija.
- **Dodatno:** fullscreen režim, ograničenje na 75 FPS, izlazak pomoću `Esc`, kursor u obliku filmske kamere i poluprovidni potpis studenta.

## 🎮 Kontrole

| Ulaz | Akcija |
|---|---|
| Levi klik na plavo sedište | Rezerviši sedište |
| Levi klik na žuto sedište | Otkaži rezervaciju |
| `1`–`9` | Kupi N susednih slobodnih sedišta |
| `Enter` | Pokreni simulaciju |
| `Esc` | Zatvori aplikaciju |

> Tokom simulacije rezervacija i kupovina su onemogućene. Kupljena (crvena) sedišta ne mogu se menjati klikom.

## 🛠️ Tehnologije

- **C++**
- **OpenGL 3.3 Core Profile** i **GLSL** šejderi
- **GLFW** — prozor, tastatura, miš i vreme animacija
- **GLEW** — pristup OpenGL funkcijama
- **stb_image** — učitavanje PNG tekstura
- **Visual Studio 2022** — razvoj i kompajliranje na Windowsu

## 🚀 Pokretanje

1. Otvoriti **`Kostur/Kostur.sln`** u programu **Visual Studio 2022**.
2. Izabrati konfiguraciju **Debug | x64**.
3. Po potrebi obnoviti NuGet pakete za **GLFW** i **GLEW** (*Restore NuGet Packages*).
4. Kompajlirati projekat pomoću **Build → Build Solution** (`Ctrl + Shift + B`).
5. Pokrenuti aplikaciju pomoću **Ctrl + F5**.

**Napomena:** Šejderi i slike učitavaju se relativnim putanjama (`Shaders/...`, `Resources/...`), pa radni direktorijum pri pokretanju treba da bude folder projekta **`Kostur/`**. U Visual Studiju se po potrebi podešava kroz *Project Properties → Debugging → Working Directory*.

## 📁 Organizacija projekta

```text
Kostur/
├── Header/
│   ├── Util.h
│   ├── Seats.h
│   ├── Cinema.h
│   ├── People.h
│   └── Signature.h
├── Source/
│   ├── Main.cpp
│   ├── Util.cpp
│   ├── Seats.cpp
│   ├── Cinema.cpp
│   ├── People.cpp
│   └── Signature.cpp
├── Shaders/
│   ├── basic.vert
│   ├── basic.frag
│   ├── people.vert
│   ├── people.frag
│   └── screen.frag
├── Resources/
│   ├── person.png
│   ├── camera_cursor_64.png
│   └── signature.png
└── Kostur.sln
```

- **`Main.cpp`** — inicijalizacija, glavna petlja i upravljanje fazama simulacije.
- **`Seats`** — statusi sedišta, rezervacija i kupovina.
- **`Cinema`** — zidovi, vrata i poluprovidni sloj.
- **`People`** — dodela mesta, PNG prikaz i animacije gledalaca.
- **`Signature`** — prikaz potpisa u uglu ekrana.
- **`Shaders`** — iscrtavanje obojenih i teksturiranih objekata.

## 🔄 Tok simulacije

```text
Rezervacija i kupovina
         ↓ Enter
Ulazak gledalaca
         ↓ Svi su seli
Projekcija (≥ 20 sekundi)
         ↓
Izlazak gledalaca
         ↓ Svi su izašli
Povratak na rezervaciju i kupovinu
```

---

*Projekat je realizovan kao 2D OpenGL aplikacija u okviru predmeta Računarska grafika.*
