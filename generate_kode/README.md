## kode booting

Saatnya untuk beberapa kode, meskipun beban kernel kita akan ditulis dalam C,  
ada hal-hal tertentu yang harus kita gunakan untuk perakitan. Salah satunya adalah kode 
boot awal.

```s
; boot.s lokasi awal dari kernel. juga mendifinisikan header multiboot.
; berdasarkan file tutorial pengembangan kernel

MBOOT_PAGE_ALIGN equ 1 << 0 ; muat kernel dengan modul batas halaman
MBOOT_MEM_INFO equ 1 << 1
MBOOT_HEADER_MAGIC equ 0x1BADB002 
; note : kita tidak menggunakan MBOOT_AOUT_KLUDGE. artinya
; GRUB berikan kami tabel
MBOOT_HEADER_FLAGS equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32] ; semua intruksi harus ke dalam 32 bit
[GLOBAL MBOOT] ; menjadikan 'mboot' dapat diakses dari C.
[EXTERN code] ; mulai dari bagian '.text'
[EXTERN bss] ; mulai dari bagian .bss
[EXTERN end] ; akhir dari bagian terakhir yang dapat dimuat.

mboot:
    dd MBOOT_HEADER_MAGIC ; GRUB akan mencari nilai ini pada setiap
                            ; batas 4-byte dalam file kernel kita
    dd MBOOT_HEADER_FLAGS ; bagaimana seharusnya GRUB memuat file / pengaturan kita
    dd MBOOT_CHECKSUM ; untuk memastikan bahwa nilai di atas benar
    
    dd mboot ; lokasi dari deskriptor
    dd code ; start dari kernel '.text' section
    dd bss ; akhir dari section kernel '.data'
    dd end ; akhir dari kernel
    dd start ; entri point kernel
    
[GLOBAL start]
[EXTERN main]

start:
    push ebx ; memuat lokasi header multiboot

    ; jalankan kernel
    cli
    call main
    jmp $
    
```
## multiboot

multiboot adalah standar yang GRUB harapkan untuk dipatuhi oleh kernel. Ini adalah cara bagi bootloader
untuk 
1. ketahui dengan tepat environment apa yang diinginkan/dibutukan kernel saat boot.
2. megizinkan kernel untuk ask environment dimana.

Jadi, misalnya, jika kernel kita perlu dimuat dalam mode VESA tertentu, kita dapat membaritahu bootloader tentang hal 
ini, dan bootloader akan menanganinya untuk kita.

Untuk membuat kernel multiboot kita kompatible, kita perlu menambahkan struktur header di suatu tempat di kernel kita  (
sebenarnya, header kita harus dalam 4KB pertama dari kernel).

```
dd MBOOT_HEADER_MAGIC
dd MBOOT_HEADER_FLAGS
dd MBOOT_CHECKSUM
dd mboot
dd code
dd bss
dd end
dd start
```
## menambahkan beberapa kode C
menghubungkan kode assembly dengan C sangat mudah. anda hanya perlu mengetahui konvensi pemanggilan yang digunakan. 
GCC pada x86 menggunakan konvesi pemanggilan ``__cdecl``
- semua parameter ke suatu fungsi dilewatkan pada tumpukan.
- parameter didorong dari kanan ke kiri.
- nilai return fungsi dikembalikan dalam EAX.

... jadi fungsi panggilan:
```
d = func(a, b, c);
```

menjadi
```
push [c]
push [b]
push [a]
call func
mov [d], eax
```

```c
// main.c -> mendifinisikan titik masuk kernel kode-C, memanggil rutinitas inisialisasi.

int main(struct multiboot * mboot_ptr){
    // semua panggilan inisialisasi kita masuk ke sini.
    return 0xDEADBABA;
}
```

inilah inkarnasi pertama kita dari fungsi ``main()``. Seperti yang kita lihat, kita membuatnya mengambil suatu parameter - 
pointer ke struct multiboot.kita akan mendifinisikannya nanti (kita sebenarnya tidak perlu mendifinisikannya agar kode dapat dikompilasi).

semua fungsi yang dilakukan adalah mengembalikan kostanta - 0xDEADBABA. Kostanta itu cukup tidak biasa sehingga harus menonjol pada
kita ketika kita menjalankan program dalam sedetik.

## mengkopilasi, menautkan, dan menjalankan

sekarang kita menambahkan file baru ke proyek kita, kita juga harus menambahkannya ke ``Makefile``. edit baris ini
```
SOURCES=boot.o
CFLAGS=
```

menjadi

```
SOURCES=boot.o main.o
CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector
```

kita harus menghentikan GCC yang mencoba menautkan pustaka linx C anda dengan kernel kita - itu tidak akan berfugnsi sama sekali.
untuk itulah ``CFLAGS`` itu.

sekarang kita sudah dapat mengkompilasi, menatukan, dan menjalankan kernel kita

```
cd src
make clean
make
cd ..
./update_image.sh
./run_bochs.sh
```

itu akan menyebabkan bochs untuk boot, kita akan melihat GRUB selama beberapa detik kemudian kernel akan berjalan.
Itu tidak benar-benar melakukan apa-apa, jadi itu akan freeze.

Jika anda membuat ``bochsout.txt``, di bagian bawah kita akan melihat sesuatu seperti
```
00074621500i[CPU ] | EAX=deadbaba EBX=0002d000 ECX=0001edd0 EDX=00000001
0074621500i[CPU ] | ESP=00067ec8 EBP=00067ee0 ESI=00053c76 EDI=00053c77
00074621500i[CPU ] | IOPL=0 id vip vif ac vm rf nt dari df jika tf sf zf af pf cf
0074621500i[CPU ] | Batas dasar pemilih SEG
GD 00074621500i[CPU ] | SEG sltr(indeks|ti|rpl) batas dasar G D
00074621500i[CPU ] | CS:0008( 0001| 0| 0) 00000000 000fffff 1 1
0074621500i[CPU ] | DS:0010( 0002| 0| 0) 00000000 000fffff 1 1
00074621500i[CPU ] | SS:0010( 0002| 0| 0) 00000000 000fffff 1 1
00074621500i[CPU ] | ES:0010( 0002| 0| 0) 00000000 000fffff 1 1
00074621500i[CPU ] | FS:0010( 0002| 0| 0) 00000000 000fffff 1 1
00074621500i[CPU ] | GS:0010( 0002| 0| 0) 00000000 000fffff 1 1
00074621500i[CPU ] | EIP=00100027 (00100027)
00074621500i[CPU ] | CR0=0x00000011 CR1=0 CR2=0x00000000
0074621500i[CPU ] | CR3=0x00000000 CR4=0x00000000
0074621500i[CPU ] >> jmp .+0xffffffffe (0x00100027) : EBFE
```

![genesis](../.github/generate_kode/genesis_bochs.png)

Perhatikan berapa nilai EAX? 0xDEADBABA - nilai kembalian ``main()``. selamat, kita sekarang memiliki multiboot trampolin assembly yang 
kompatibel, dan kita siap untuk mulai display kepada layar.
