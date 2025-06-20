/*
    -- Proyek IoT Suhu & Kelembaban --
    
    Kode ini membaca data dari sensor DHT (DHT11/DHT22) dan menampilkannya
    secara nirkabel di aplikasi mobile menggunakan RemoteXY.
    
    Pastikan Anda sudah menginstall library 'RemoteXY' dan 'DHT sensor library' by Adafruit.
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// Anda bisa mengaktifkan debug log ke Serial Monitor pada baud rate 115200
//#define REMOTEXY__DEBUGLOG

// Pilih mode koneksi dan sertakan library RemoteXY
#define REMOTEXY_MODE__WIFI_POINT
#include <ESP8266WiFi.h>

// Pengaturan koneksi RemoteXY 
#define REMOTEXY_WIFI_SSID "Rainnly"
#define REMOTEXY_WIFI_PASSWORD "123456789"
#define REMOTEXY_SERVER_PORT 6377

#include <RemoteXY.h>

// Konfigurasi GUI RemoteXY
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 72 bytes
  { 255,0,0,8,0,65,0,19,0,0,0,0,31,1,106,200,1,1,4,0,
  67,34,58,40,10,78,2,26,2,67,36,110,40,10,78,2,26,2,129,36,
  38,33,12,64,17,83,117,104,117,32,58,0,129,21,92,73,12,64,17,75,
  101,108,101,109,98,97,98,97,110,32,58,0 };
  
// Struktur ini mendefinisikan semua variabel dan event dari antarmuka kontrol Anda
struct {
    // Variabel output (untuk ditampilkan di aplikasi)
    float value_01; // Akan kita gunakan untuk menampilkan SUHU
    float value_02; // Akan kita gunakan untuk menampilkan KELEMBABAN

    // Variabel lain
    uint8_t connect_flag;  // =1 jika aplikasi terhubung, else =0
} RemoteXY;
#pragma pack(pop)


/////////////////////////////////////////////
//        Tambahan untuk Sensor DHT        //
/////////////////////////////////////////////
#include <DHT.h>

// Tentukan pin mana yang terhubung dengan sensor DHT
#define DHTPIN D4     // Pin D4 pada NodeMCU (GPIO2)

// Tentukan tipe sensor DHT yang Anda gunakan
#define DHTTYPE DHT11       // Untuk sensor DHT 11 (biru)
// #define DHTTYPE DHT22      // Untuk sensor DHT 22 (putih, AM2302). Hapus komentar jika menggunakan ini.

// Inisialisasi objek DHT
DHT dht(DHTPIN, DHTTYPE);


/////////////////////////////////////////////
//              Fungsi Setup               //
/////////////////////////////////////////////
void setup() 
{
  RemoteXY_Init(); 
  
  // Memulai sensor DHT
  dht.begin();
  
  // (Opsional) Memulai Serial Monitor untuk debugging
  Serial.begin(115200);
}

/////////////////////////////////////////////
//              Fungsi Loop                //
/////////////////////////////////////////////
void loop() 
{ 
  RemoteXY_Handler();
  
  // Beri sedikit jeda agar pembacaan sensor stabil dan tidak terlalu sering
  // Sensor DHT direkomendasikan untuk dibaca setiap 2 detik sekali.
  delay(2000); 
  
  // Baca kelembaban dari sensor
  float h = dht.readHumidity();
  // Baca suhu dalam Celsius dari sensor
  float t = dht.readTemperature();

  // Cek apakah pembacaan gagal. Jika gagal, hentikan eksekusi (untuk menghindari data aneh).
  if (isnan(h) || isnan(t)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    return;
  }
  
  // Jika pembacaan berhasil, masukkan nilainya ke dalam struktur RemoteXY
  RemoteXY.value_01 = t;      // Kirim data suhu ke elemen pertama di GUI
  RemoteXY.value_02 = h;      // Kirim data kelembaban ke elemen kedua di GUI

  // (Opsional) Tampilkan data di Serial Monitor untuk pengecekan
  Serial.print("Suhu: ");
  Serial.print(t);
  Serial.print(" °C\t");
  Serial.print("Kelembaban: ");
  Serial.print(h);
  Serial.println(" %");
}
