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
// Забороняємо компілятору зливати (inline) цю функцію, щоб вона чітко відображалась на Flame Graph
[[gnu::noinline]]
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
// Забороняємо компілятору зливати (inline) цю функцію, щоб вона чітко відображалась на Flame Graph
[[gnu::noinline]]
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

---

## 6. Магія векторизації (SIMD) та чому i-k-j працює ще швидше

Ми вже з'ясували, що цикл `i-k-j` краще працює з кешем (нашим "столом" для книг). Але це ще не все! Процесор має ще один потужний інструмент під назвою **SIMD** (Single Instruction, Multiple Data — одна інструкція, багато даних).

### Аналогія з життя: Студент із супер-маркером
Уявіть, що студенту треба знайти і закреслити переклади 4 різних слів у 4 різних словниках.
- **Звичайний підхід (Скалярний):** Студент бере перший словник, знаходить слово, закреслює його. Кладе. Бере другий словник, шукає, закреслює... Це довго, бо кожна дія робиться по черзі (одна дія = одне слово).
- **Підхід SIMD (Векторний):** Студент викладає 4 словники рівно поруч на столі. Бере спеціальний "широкий" маркер і за **один рух руки** закреслює 4 слова одразу у всіх чотирьох книжках! Це робиться в 4 рази швидше, рівно за той самий час, що потрібен для закреслення одного слова.

### SIMD у процесорі
Ваш комп'ютер вміє робити такі ж супер-рухи! Замість того, щоб додавати числа по одному (наприклад, `1 + 5`), сучасні процесори мають широкі "регістри" (як той широкий маркер), що дозволяють захопити одразу 4 або навіть 8 чисел підряд і виконати додавання або множення за один такт процесора!

Але є **одне дуже суворе правило**: щоб скористатися широким маркером, книги на столі (дані в пам'яті) мають лежати **ідеально рівно та підряд**!

### Як це працює у i-k-j
Поглянемо ще раз на найвнутрішніший цикл `j` у нашому швидкому варіанті:
```cpp
// A[i][k] — незмінне число для цього циклу (наш константний множник)
// B та C читаються підряд по горизонталі: j, j+1, j+2...
C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
```
Оскільки `B` та `C` лежать в оперативній пам'яті ідеально підряд (як ті словники в ряд), компілятор може використати SIMD-інструкції (процесорні "супер-маркери" типу AVX). Він бере одне число `A`, одразу 4 числа з `B`, одночасно їх перемножує, і так само одночасно додає до 4-х чисел із `C`!

У "повільному" циклі `i-j-k` ми постійно стрибали по вертикалі. Дані не лежали підряд. Студенту з "широким маркером" там просто нічого робити, тому процесор відмовляється від такої оптимізації і робить все по одному числу, що дуже довго.

Як бачите, розуміння того, як дані лежать у пам'яті комп'ютера, дозволяє не лише швидше їх завантажувати, а й застосовувати "суперздібності" процесора!

<style>
.simd-wrap { display: flex; flex-direction: column; align-items: center; border: 1px solid #ddd; padding: 10px; border-radius: 8px; margin-bottom: 20px; background: #fdfdfd;}
.simd-box { display: flex; gap: 4px; padding: 6px; border: 2px solid #ccc; border-radius: 5px; background: #fff;}
.simd-cell { width: 30px; height: 30px; display: flex; align-items: center; justify-content: center; border: 1px solid #eee; font-family: monospace; font-size: 14px; transition: all 0.2s;}
.simd-avx { background-color: #ffe6e6; border-color: #c00; font-weight: bold; transform: scale(1.1); box-shadow: 0 0 10px rgba(200,0,0,0.3);}
.simd-scalar { background-color: #e6e6ff; border-color: #00c; font-weight: bold; transform: scale(1.1);}
</style>

<div class="simd-wrap">
  <h3>Візуалізація: Звичайний цикл проти SIMD</h3>
  <div style="display: flex; gap: 30px; margin-bottom: 20px; flex-wrap: wrap; justify-content: center;">
    
    <div style="text-align: center;">
      <b>Скалярний режим (по одному)</b><br><br>
      <div class="simd-box" id="scalar-box">
      </div>
      <div id="scalar-info" style="margin-top: 10px; font-weight: bold; color: #00c; height: 30px;"></div>
    </div>
    
    <div style="text-align: center;">
      <b>SIMD векторний режим (по 4 одразу)</b><br><br>
      <div class="simd-box" id="vector-box">
      </div>
      <div id="vector-info" style="margin-top: 10px; font-weight: bold; color: #c00; height: 30px;"></div>
    </div>
    
  </div>
  <div class="controls">
    <button onclick="runSimdAnim()" style="background: #e67e22;">▶ Запустити порівняння</button>
  </div>
</div>

<script>
function buildSimdBox(id) {
  const c = document.getElementById(id);
  c.innerHTML = '';
  for(let i=0; i<8; i++) {
    let el = document.createElement('div'); el.className = 'simd-cell'; el.id = `${id}-${i}`; el.innerText = i; c.appendChild(el);
  }
}
buildSimdBox('scalar-box'); buildSimdBox('vector-box');

let simdTimer1 = null;
let simdTimer2 = null;

function runSimdAnim() {
  clearInterval(simdTimer1); clearInterval(simdTimer2);
  document.querySelectorAll('#scalar-box .simd-cell, #vector-box .simd-cell').forEach(e => e.className = 'simd-cell');
  
  let s_idx = 0;
  let v_idx = 0;
  
  // Скалярний - 1 елемент за один крок (повільніше)
  simdTimer1 = setInterval(() => {
    document.querySelectorAll('#scalar-box .simd-cell').forEach(e => e.className = 'simd-cell');
    if (s_idx < 8) {
      document.getElementById(`scalar-box-${s_idx}`).classList.add('simd-scalar');
      document.getElementById('scalar-info').innerText = `Такт ${s_idx+1}: обробка [${s_idx}]`;
      s_idx++;
    } else {
      document.getElementById('scalar-info').innerText = `Готово за 8 тактів!`;
      clearInterval(simdTimer1);
    }
  }, 500);
  
  // Векторний (SIMD) - 4 елементи за один крок (швидше)
  simdTimer2 = setInterval(() => {
    document.querySelectorAll('#vector-box .simd-cell').forEach(e => e.className = 'simd-cell');
    if (v_idx < 8) {
      for(let i=0; i<4; i++) {
        if(v_idx+i < 8) document.getElementById(`vector-box-${v_idx+i}`).classList.add('simd-avx');
      }
      document.getElementById('vector-info').innerText = `Такт ${(v_idx/4)+1}: обробка [${v_idx}...${v_idx+3}]`;
      v_idx += 4;
    } else {
      document.getElementById('vector-info').innerText = `Готово за 2 такти! (Швидше!)`;
      clearInterval(simdTimer2);
    }
  }, 500);
}
</script>

---

## 7. Практика: Дивимося на кеш-промахи своїми очима (Flame Graphs)

Ми вже знаємо теорію про "широкі штампи" та читання "підряд по рядках", але як подивитися всередину справжнього процесора під час роботи алгоритму? 

Для цього існують спеціальні інструменти. Наприклад, в системах Linux є потужний профілювальник під назвою `perf`. Він вміє відстежувати буквально кожен чих процесора, зокрема рахувати **кеш-промахи (Cache Misses)**. Ми запустили наші алгоритми через `perf` і зібрали дані спеціально для цієї лекції.

Щоб візуалізувати ці тисячі рядків нудних чисел, була придумана геніальна візуалізація — **Flame Graphs (Флейм-графи** / **Полум'яні графи)**. Вони показують, яка частина коду займає найбільше часу або генерує найбільше подій (чи тих самих промахів).

### Наш інтерактивний Flame Graph промахів

<iframe src="./speedscope/index.html#profileURL=../profile.txt" width="100%" height="600" style="border: 2px solid #ccc; border-radius: 8px; margin-bottom: 20px;"></iframe>

> Якщо вікно залишилось синім і графік не відобразився (іноді так буває через налаштування браузера), просто [👉 збережіть файл profile.txt](./profile.txt) на комп'ютер та перетягніть його самостійно всередину Speedscope вище.

### Що ви там побачите (і як це читати)?
Уявіть, що Flame Graph — це розріз вашого коду як листкового пирога. 
- **Ширина блоку (Це найголовніше!):** показує, **скільки кеш-промахів** згенерувала ця конкретна функція! Чим ширший блок — тим частіше процесор спотикався, чекаючи на повільну пам'ять. На нашому експерименті (N=2048) `i-j-k` зайняв 162 сек, а `i-k-j` лише 8.9 сек (у 18 разів швидше), що ідеально видно на графіку.
- **Зверху вниз (по осі Y):** показується ланцюг того, як функції викликали одна одну (Стек викликів).

**Що треба знайти на графіку:**
Перемкніться на вкладку **"Left Heavy"** (вгорі зліва екрана Speedscope). Цей режим групує всі виклики. Порівняйте такі елементи:

1. **Масивний блок `multiplyMatrixIJK`:** Це наш повільний метод. Він займає близько **95% всієї ширини** графіка. Він згенерував левову частку всіх кеш-промахів через свої неефективні стрибки по вертикалі матриці B.
2. **Вузький блок `multiplyMatrixIKJ`:** Це оптимізований метод (праворуч екрана, можливо скорочений до `mul...KJ`). Він займає лише близько **1–5% ширини**, оскільки читав дані ідеально підряд, і кеш-промахів там майже не було.
3. **Глибокий "сталактит викликів" зі знаками питання `???` (або `[unknown]`):** Під масивним блоком `multiplyMatrixIJK` ви побачите довгий стовпчик функцій, що тягнеться донизу. Що це таке? Коли виникає жорсткий промах кешу і пам'ять доводиться підвантажувати з ОЗП (Page Fault), комп'ютер "провалюється" з вашого коду на рівень ядра операційної системи Linux. Ядро містить глибокий ланцюг функцій порятунку та виділення пам'яті. `perf` записав увесь цей шлях "провалювання" процесора, проте, оскільки ми збирали дані без прав адміністратора, ядро приховало імена своїх функцій з міркувань безпеки (тому і `???`). Цей стовпчик є прямим доказом і візуалізацією того важкого шляху, який змушена долати операційна система, щоб виправити наслідки вашого неоптимального коду!

*Ось так математичні теорії втілюються в інженерії: просто змінивши порядок двох циклів, ми прибрали величезне вузьке гірло і зробили комп'ютеру "приємно", а нашу програму — значно швидшою!*

