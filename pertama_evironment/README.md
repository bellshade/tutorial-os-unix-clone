# Environment

kita membutuhkan basis untuk merancang dan membuat kernel. disini kita akan 
mengasumsikan bahwa kita menggunakan operasi sistem linux atau unix, dengant toolchain GNU. jika
menggunakan operasi sistem windows, maka harus menggunakan ``cygwin`` yang merupakan emulasi dari terminal 
*nix

## struktur direktor

```
pembahasan
 |
 + -- sumber kode
 |
 + -- penjelasan
```

## kompilasi

pada pembahasan ini kita harus bisa hasil sumber kode dikompilasi dengan toolchain GNU (gcc, ld, dll).
contoh assembly ditulis dalam sintaks intel, karena sintaks dari intel jauh lebih mudah dan dapata dibaca 
daripada sintaks assembly dari AT&T yang menggunakan GNU AS, untuk assembly ini kita menggunakan [netwide assembler](https://www.nasm.us/)

## running

Bochs adalah emulator x86-64 yang bersifat open source, ketika running fail, bochs akan mengistruksikan kita dan menyimpan status 
prosesor  dalam filelog, yang sangat berguna. Juga dapat dijalankan dan di reboot jauh lebuh cepat dari mesin yang nyata

### Bochs 

untuk menjalankan bochs, kita memerlukan file konfigurasi bochs (``bocshrc.txt``).
**berhati hatilah dengan lokasi file bios**. 

```txt
megs: 32
romimage: file=/usr/share/bochs/BIOS-bochs-latest, address=0xf0000
vgaromimage: /usr/share/bochs/VGABIOS-elpin-2.40
floppya: 1_44=/dev/loop0, status=inserted
boot: a
log: bochsout.txt
mouse: enabled=0
clock: sync=realtime
cpu: ips=500000
```

Ini akan membuat bochs meniru mesin 32MB dengan kecepatan clock yang mirip dengan Pentium 350Mhz. Instruksi per detik dapat juga ditingkatkan. disini
kecepatan emulasi dibuat lebih lambat, hanya agar  kita dapat melihat apa yang terjadi saat proses tersebut berjalan

### Makefile

kitaakan sering melakukan beberapa hal, membuat (mengkompilasi dan mengguhubungkan) project akita, dan mentrasfer biner kernel yang dihasilkan image
disk kita

```Makefile
SOURCES =boot.o

CFLAGS =
LDFLAGS =-Tlink.ld
ASFLAGS =-felf

semua:  $( SOURCES ) link 

clean:
  - rm  *.o kernel

link:
 ld $( LDFLAGS ) -o kernel $( SOURCES )

.s .o:
 nasm $( ASFLAGS ) $ <
```

makefile ini akan mengkompilasi setiap file dalam ``SOURCES``, kemudian akan menatukannya menjadi satu biner
ELF, 'kernel'. proses ini menggunakan skrip tautan yang bernama ``link.ld``


### Link.ld
```
ENTRY(start)
SECTIONS
{
  .text 0x100000 :
  {
    code = .; _code = .; __code = .;
    *(.text)
    . = ALIGN(4096);
  }

  .data :
  {
     data = .; _data = .; __data = .;
     *(.data)
     *(.rodata)
     . = ALIGN(4096);
  }

  .bss :
  {
    bss = .; _bss = .; __bss = .;
    *(.bss)
    . = ALIGN(4096);
  }

  end = .; _end = .; __end = .;
}
```

skrip ini memberitahu LD (load doubleword) cara mengatur image kernel kita. pertama, ia memberitahkan LD bahwa lokasi awal biner kita harus menjadi 
simbol 'start'. ini kemudian memberi tahu LD bahwa bagian ``.text`` (di situlah semua kode kita) harus menjadi yang pertama, dan harus dimulai dari 
``0x100000`` (1MB). ``.data`` (data statis yang akan diinisialisai) dan ``.bss`` (data statis yang tidak diinisialisasi) harus berikutnya, dan masing masing 
harus disejajarkan dengan halaman (``ALIGN(4096)``). ``.rodata`` ini untuk data yang diinisialisai hanya baca seperti kostanta. untuk kesederhanaan maka di bagian kita gabungkan
``.data``


### ``update_image.sh``

skrip sederhana ini yang akan memaskan biner kernel baru kita ke dalam image floppy (ini mengasumsukan kita telah membuat direktor ``/mnt``)
**note**: kamu perlu ``/sbin`` di  ``$PATH`` kamu untuk losetup

```sh
#!/bin/bash

sudo losetup /dev/loop0 floppy.img
sudo mount /dev/loop0 /mnt
sudo cp src/kernel /mnt/kernel
sudo umount /dev/loop0
sudo losetup -d /dev/loop0
```

### ``run_bochs.sh``

skrip sederhana dibawah ini akan menyiapkan loopback, kemudian menjalankan boch di dalamnnya

```sh
#!bin/bash

sudo  /sbin/ losetup  /dev/loop0 floppy.img
sudo  bochs -f bochsrc.txt
sudo  /sbin/ losetup  -d /dev/loop0
```
