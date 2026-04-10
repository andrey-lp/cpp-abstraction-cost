# Як вибір порядку циклів впливає на швидкість: Порівняння алгоритмів множення матриць

Коли ми переносимо математичні формули у код, важливо пам'ятати, що комп'ютер має свої правила роботи з пам'яттю. У цій статті ми дізнаємося, чому зміна порядку звичайних циклів `for` може пришвидшити вашу програму у кілька разів.

---

## 1. Класичне математичне множення (цикл i-j-k)

В математиці множення двох матриць **A** та **B** зводиться до обчислення кожного елементу матриці-результату **C**. Кожен елемент `C[i][j]` — це сума добутків відповідного *рядка* першої матриці на *стовпець* другої.

Давайте подивимося на це візуально на прикладі матриці 3х3:

<style>
.mat-wrap { display: flex; flex-direction: column; align-items: center; border: 1px solid #ddd; padding: 10px; border-radius: 8px; margin-bottom: 20px; background: #fdfdfd;}
.mat-row-flex { display: flex; align-items: center; justify-content: center; gap: 10px; flex-wrap: wrap; margin-bottom: 10px;}
.matrix { display: grid; grid-template-columns: repeat(3, 40px); gap: 4px; padding: 6px; border-left: 2px solid #333; border-right: 2px solid #333; border-radius: 5px; background: #fff;}
.cell { width: 40px; height: 40px; display: flex; align-items: center; justify-content: center; border: 1px solid #eee; font-family: monospace; font-size: 12px; transition: background-color 0.2s, transform 0.2s; border-radius: 4px;}
.hl-A { background-color: #ffcccc; color: #c00; font-weight: bold; border-color: #c00; transform: scale(1.1); z-index: 10;}
.hl-B { background-color: #ccffcc; color: #080; font-weight: bold; border-color: #080; transform: scale(1.1); z-index: 10;}
.hl-C { background-color: #ccccff; color: #00c; font-weight: bold; border-color: #00c; transform: scale(1.1); z-index: 10;}
.controls button { padding: 8px 16px; margin: 5px; cursor: pointer; border: none; background: #0066cc; color: white; border-radius: 4px; font-weight: bold;}
.controls button:hover { background: #0055aa; }
.controls button.stop { background: #cc0000; }
.controls button.stop:hover { background: #aa0000; }
#animData1, #animData2 { font-family: monospace; font-size: 16px; min-height: 24px; padding: 10px; background: #eee; border-radius: 4px; width: 100%; text-align: center; margin-top: 10px;}
</style>

<div class="mat-wrap">
  <div class="mat-row-flex">
    <div style="text-align:center;"><b>Матриця A</b><div class="matrix" id="m-A1"></div></div>
    <div><b>×</b></div>
    <div style="text-align:center;"><b>Матриця B</b><div class="matrix" id="m-B1"></div></div>
    <div><b>=</b></div>
    <div style="text-align:center;"><b>Матриця C</b><div class="matrix" id="m-C1"></div></div>
  </div>
  <div id="animData1">Запустіть анімацію...</div>
  <div class="controls">
    <button onclick="runAnim1()">▶ Анімація множення (i-j-k)</button>
    <button class="stop" onclick="stopAnim1()">⏹ Зупинити</button>
  </div>
</div>

<script>
function buildMat(id, label) {
  const c = document.getElementById(id); c.innerHTML = '';
  for(let i=0; i<3; i++) for(let j=0; j<3; j++) {
    let el = document.createElement('div'); el.className = 'cell'; el.id = `${id}-${i}-${j}`; el.innerText = `${label}${i}${j}`; c.appendChild(el);
  }
}
buildMat('m-A1', 'A'); buildMat('m-B1', 'B'); buildMat('m-C1', 'C');

let timer1 = null;
function stopAnim1() { 
  clearInterval(timer1); 
  document.querySelectorAll('#m-A1 .cell, #m-B1 .cell, #m-C1 .cell').forEach(e => e.className = 'cell');
  document.getElementById('animData1').innerText = '';
}
function runAnim1() {
  stopAnim1();
  let i=0, j=0, k=0;
  timer1 = setInterval(() => {
    document.querySelectorAll('#m-A1 .cell, #m-B1 .cell, #m-C1 .cell').forEach(e => e.className = 'cell');
    document.getElementById(`m-A1-${i}-${k}`).classList.add('hl-A');
    document.getElementById(`m-B1-${k}-${j}`).classList.add('hl-B');
    document.getElementById(`m-C1-${i}-${j}`).classList.add('hl-C');
    document.getElementById('animData1').innerText = `C[${i}][${j}] = C[${i}][${j}] + A[${i}][${k}] * B[${k}][${j}]`;
    k++;
    if(k > 2) { k = 0; j++; }
    if(j > 2) { j = 0; i++; }
    if(i > 2) { clearInterval(timer1); setTimeout(stopAnim1, 1500); }
  }, 600);
}
</script>

Ви могли помітити, як в анімації ми біжимо:
- **Горизонтально** по матриці **A** (читаємо елементи підряд: `A00`, `A01`, `A02`).
- **Вертикально** по матриці **B** (читаємо елементи зі "стрибками": `B00`, `B10`, `B20`).

Для математики це чудово. Але для комп'ютера обхід "по стовпцях" (вертикально) є надзвичайно неефективним! Чому? Все через кеш процесора.

---

## 2. Кеш процесора та аналогія з бібліотекою

Уявіть, що процесор — це студент, який сидить за столом і вчить матеріал, а оперативна пам'ять (RAM) — це величезний архів у підвалі бібліотеки. Швидкість доступу до підвалу дуже повільна.

Щоб не бігати в підвал по кожну окрему сторінку, студент приносить інформацію **цілими книгами** або **стосами книг**. Стіл студента, на якому зберігається кілька зараз потрібних книг — це і є **кеш процесора (Cache)**. 

Коли процесору потрібен один елемент матриці (одна сторінка), він завантажує з повільного підвалу одразу цілий "стос" сусідніх елементів (цілу **кеш-лінію**, що вміщує наприклад 16 чисел підряд).

- **Вдалий випадок (Cache Hit):** Коли ми читаємо елементи рядка один за одним (горизонтально), перший доступ доведеться почекати, але наступні 15 чисел вже лежать на столі! Це миттєво.
- **Погана ситуація (Cache Miss):** Коли ми стрибаємо різними рядками (наприклад йдемо по стовпцю вниз, як ми робили з матрицею B в анімації вище), потрібний нам елемент лежить в **іншій книзі**. Процесор постійно бігає у підвал по нові книги, а ті, що лежать на столі — ігнорує і викидає.

Саме такі стрибки в пам'яті "вбивають" швидкість програми.

---

## 3. Оптимізований алгоритм (i-k-j) і "дружність" до кешу

Як же обманути систему? Математика дозволяє нам переставити порядок додавання у рівняннях. Якщо замість структури `i -> j -> k` ми оберемо `i -> k -> j`, ми змінимо напрямок обходу матриць.

<div class="mat-wrap">
  <div class="mat-row-flex">
    <div style="text-align:center;"><b>Матриця A</b><div class="matrix" id="m-A2"></div></div>
    <div><b>×</b></div>
    <div style="text-align:center;"><b>Матриця B</b><div class="matrix" id="m-B2"></div></div>
    <div><b>=</b></div>
    <div style="text-align:center;"><b>Матриця C</b><div class="matrix" id="m-C2"></div></div>
  </div>
  <div id="animData2">Запустіть оптимізовану анімацію...</div>
  <div class="controls">
    <button onclick="runAnim2()" style="background: #008800;">▶ Анімація множення (i-k-j)</button>
    <button class="stop" onclick="stopAnim2()">⏹ Зупинити</button>
  </div>
</div>

<script>
buildMat('m-A2', 'A'); buildMat('m-B2', 'B'); buildMat('m-C2', 'C');

let timer2 = null;
function stopAnim2() { 
  clearInterval(timer2); 
  document.querySelectorAll('#m-A2 .cell, #m-B2 .cell, #m-C2 .cell').forEach(e => e.className = 'cell');
  document.getElementById('animData2').innerText = '';
}
function runAnim2() {
  stopAnim2();
  let i=0, k=0, j=0;
  timer2 = setInterval(() => {
    document.querySelectorAll('#m-A2 .cell, #m-B2 .cell, #m-C2 .cell').forEach(e => e.className = 'cell');
    document.getElementById(`m-A2-${i}-${k}`).classList.add('hl-A');
    document.getElementById(`m-B2-${k}-${j}`).classList.add('hl-B');
    document.getElementById(`m-C2-${i}-${j}`).classList.add('hl-C');
    document.getElementById('animData2').innerText = `C[${i}][${j}] = C[${i}][${j}] + A[${i}][${k}] * B[${k}][${j}]`;
    j++;
    if(j > 2) { j = 0; k++; }
    if(k > 2) { k = 0; i++; }
    if(i > 2) { clearInterval(timer2); setTimeout(stopAnim2, 1500); }
  }, 400); // Зробив навіть швидше візуально, бо кеш-хіти!
}
</script>

Зверніть увагу на оптимізовану анімацію!
Що відбувається у найвнутрішнішому швидкому циклі (коли змінюється `j`):
1. **Матриця A:** Елемент `A[i][k]` залишається на місці (константа). Ми його один раз поклали "на стіл" і просто використовуємо.
2. **Матриця B:** Змінюється лише `j` (другий індекс), тобто ми йдемо красивим горизонтальним рядком і з ідеальною швидкістю читаємо елементи прямо з нашого "стосу".
3. **Матриця C:** Також змінюється лише `j`. Ми просто послідовно перезаписуємо елемент за елементом у рядку!

Всі звернення до пам'яті відбуваються сумарно по горизонталі. Процесор знаходиться на піку продуктивності!

---

## 4. Порівняльна таблиця методів

Давайте підіб'ємо підсумки двох підходів до множення матриць мовою комп'ютера:

| Характеристика | Базовий спосіб: `i - j - k` | Оптимізований спосіб: `i - k - j` |
| --- | --- | --- |
| **Структура циклів** | Внутрішній цикл проходить по `k` | Внутрішній цикл проходить по `j` |
| **Доступ до `Матриці A`** | Послідовний (горизонтальний) ✅ | Фіксований елемент у внутр. циклі ✅✅ |
| **Доступ до `Матриці B`** | **Стрибками (по стовпцю вниз) ❌** | Послідовний (горизонтальний) ✅ |
| **Доступ до `Матриці C`** | Фіксований елемент у внутр. циклі ✅✅ | Послідовний (горизонтальний) ✅ |
| **Ефективність кешу (Cache Hits)** | Низька (багато дорогих промахів) | Максимально висока |
| **Очікувана швидкість C++** | Повільно | В ~3–5 разів швидше! 🚀 |

Як бачимо, навіть не змінюючи загальної кількості математичних операцій, розуміння архітектури комп'ютера дозволяє нам писати оптимізований код, який повноцінно розкриває можливості "заліза".

---

## 5. C++ Code Snippets (Приклади реалізації)

Ось процедурна реалізація на C++ для обох алгоритмів. Зверніть увагу, як змінюється лише порядок циклів `for`, але це критично впливає на швидкодію через роботу з кешем.

### Класичний "Повільний" Алгоритм (i-j-k)
```cpp
void multiplyMatrixIJK(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int n = A.size();
    
    // Loop through the rows of Matrix A
    for (int i = 0; i < n; i++) {
        // Loop through the columns of Matrix B
        for (int j = 0; j < n; j++) {
            // Loop through the elements to calculate the dot product
            for (int k = 0; k < n; k++) {
                // Here, B is accessed column by column: B[k][j]
                // This causes "Cache Misses" because elements in a column are far apart in memory!
                C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
            }
        }
    }
}
```

### Оптимізований "Швидкий" Алгоритм (i-k-j)
```cpp
void multiplyMatrixIKJ(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int n = A.size();
    
    // Loop through the rows of Matrix A
    for (int i = 0; i < n; i++) {
        // Loop through the columns of Matrix A / rows of Matrix B
        for (int k = 0; k < n; k++) {
            // Here, A[i][k] is a constant for the innermost loop.
            // Loop through the columns of Matrix B
            for (int j = 0; j < n; j++) {
                // Here, B and C are accessed row by row: B[k][j]
                // Elements in a row are next to each other in memory.
                // This causes "Cache Hits", making the CPU read memory extremely fast!
                C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
            }
        }
    }
}
```
