# tutorial-os-unix-clone

tutorial sederhana untuk bertujuan membawa kita melalui pemograman sistem oeprasi clone dari UNIX yang sederahan untuk arsitektur x86.

contoh yang diberikan dari tutorial dan penjelasan yang diberikan sifatnya sangat praktis. algoritma yang digunakan pada pembuatan ini bukanlah yang paling hemat atau optimal, dikarenakan ini adalah sebuah pembelajaran maka diutamakan adalah kesederhanaan dan kemudahaan pemahaman.

disini kita akan membuat contoh sederhana dengan menggunakan sistem desain ``monolitik`` (driver dimuat melalui modul mode kernel sebagai dari mode pengguna), karena ini lebih sederhana. bahasa yang digunakan menggunakan bahasa ``C``sebagai  bahasa pilihan dengan basis yang dicampur dalam  bit assembler. Tujuannya adalah untuk berbicara dari desain dan keputusan implementasi dalam membuat sebuah sistem operasi.


## syarat utama

untuk kompilasi dan menjalankan kode dari tutorial, kita membutuhkan antara lain

- [``GCC``](https://gcc.gnu.org/)
- ``ld``
- ``NASM``
- ``GNU make``

**note**: harus memiliki pengetahuan yang sangat kuat tentang bahasa pemograman ``C``, terutama mengenai pointer, dan juga memiliki pengetahuan yang lebih tentang assembly (kita menggunakan assembly sintaks model intel) termasuk untuk apa fungsi dan tujuan dari register ``EBP``.

## tutorial list

list tutorial bisa dilihat dibawah ini

| indeks    | judul    | penjelasan    |
|---------------- | --------------- | --------------- |
| 1    | [setting environment](./pertama_evironment)    | setting environment sebelum memulai    |
| 2    | [generate kode](./generate_kode)    | generate kode awal untuk membuat os termasuk membuat kernel    |
| 3   | [menampilkan kata pada layar](./layar)   | hello world pertama !   |

