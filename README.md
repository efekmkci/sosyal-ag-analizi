# sosyal-ag-analizi
C ile geliştirilmiş basit bir sosyal ağ modelleme ve analiz uygulaması.

# Sosyal Ağ Modelleme ve Analizi Uygulaması (C ile)

Bu proje, C programlama dilinde geliştirilmiş basit bir sosyal ağ modelleme ve analiz uygulamasıdır. Kullanıcıları ve aralarındaki takip ilişkilerini modellemek, çeşitli sosyal ağ analizleri gerçekleştirmek ve toplulukları tespit etmek amacıyla oluşturulmuştur. Uygulamada, kullanıcı verimliliği için Kırmızı-Siyah Ağacı veri yapısı kullanılırken, ilişkiler grafik tabanlı bir yaklaşımla (bağlı listeler) temsil edilmektedir.

## Temel Özellikler

* **Kullanıcı Yönetimi:** Kullanıcı ekleme ve kimlik bilgisine göre arama işlemleri dengeli ve hızlı bir şekilde Kırmızı-Siyah Ağacı ile yönetilir.
* **Takip İlişkileri:** Kullanıcılar arasındaki takip etme ilişkileri yönlü bir grafik yapısında, bağlı listeler aracılığıyla kaydedilir.
* **Sosyal Ağ Analizleri:** Aşağıdaki temel sosyal ağ analizleri implemente edilmiştir:
    * **Belirli Mesafedeki Arkadaşları Bulma (Depth-First Search):** Bir kullanıcının belirli bir bağlantı derinliğindeki arkadaşlarını tespit eder.
    * **Ortak Arkadaş Analizi:** İki kullanıcının ortak takip ettiği kullanıcıları belirler.
    * **Etki Alanı Hesaplama:** Bir kullanıcının takip ettiği ağ üzerindeki potansiyel erişimini (etki derecesini) hesaplar.
    * **Topluluk Tespiti (Bağlantılı Bileşenler):** Ağdaki birbirine bağlı kullanıcı gruplarını (toplulukları) tespit eder.
* **Veri Yükleme:** Kullanıcı ve takip ilişkileri, basit bir metin dosyasından (`veriseti.txt`) okunarak sisteme yüklenir.

## Kullanılan Veri Yapıları

* **Kırmızı-Siyah Ağacı (Red-Black Tree):** Kullanıcıları benzersiz kimliklerine göre sıralı ve dengeli bir şekilde saklamak ve hızlı arama, ekleme, silme işlemleri yapmak için kullanılmıştır. Bu, büyük kullanıcı sayılarında bile verimli bir performans sağlar.
* **Bağlı Listeler (Linked Lists):** Her kullanıcının takip ettiği (`following`) ve onu takip eden (`followers`) kullanıcıların listesini tutmak için kullanılmıştır. Bu yapı, kullanıcılar arasındaki yönlü ilişkileri dinamik olarak yönetmeyi mümkün kılar.

## Algoritmalar

* **Depth-First Search (DFS):** Belirli mesafedeki arkadaşları bulma ve etki alanı hesaplama analizlerinde ağın derinlemesine araştırılması için kullanılmıştır.
* **Bağlantılı Bileşenler (Connected Components):** Topluluk tespiti için kullanılarak, birbirine doğrudan veya dolaylı olarak bağlı olan kullanıcıların aynı toplulukta gruplandırılması sağlanmıştır.

## `veriseti.txt` Dosya Formatı

Uygulama, kullanıcı ve takip ilişkilerini  içeren bir metin dosyasından okur:

## Nasıl Derlenir ve Çalıştırılır

1.  Bu GitHub deposunu klonlayın:
    ```bash
    git clone <depo_linkiniz>
    ```
2.  Proje dizinine gidin:
    ```bash
    cd sosyal-ag-analizi
    ```
3.  Uygulamayı bir C derleyicisi (örneğin GCC) kullanarak derleyin:
    ```bash
    gcc main.c -o sosyal_ag
    ```
4.  Uygulamayı çalıştırın:
    ```bash
    ./sosyal_ag
    ```
5.  Uygulama, sosyal ağ analizlerinin sonuçlarını ve tespit edilen toplulukları konsola yazdıracaktır.
