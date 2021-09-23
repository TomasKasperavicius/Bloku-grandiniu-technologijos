# Bloku-grandiniu-technologijos
## Naudojimosi instrukcija:
### Flag'ai:
* **-f:** išorinių failų naudojimas, pvz: ./main -f failas1.txt failas2.txt ...
* **-k:** suhash'ina konstituciją po eilutę.
* **-pairs:** testuoja hash funkciją su 100 000 simbolių eilučių porų, kurios viena nuo kitos skiriasi daugiau nei vienu simboliu.
* **-pairsDiff:** testuoja hash funkciją su 100 000 simbolių eilučių porų, kurios viena nuo kitos skiriasi tik vienu simboliu.
* **-md5:** suhash'ina duotą failą po eilutę, naudojant md5 algoritmą, pvz: ./main -md5 failas.txt
* **-sha256:** suhash'ina duotą failą po eilutę, naudojant sha256 algoritmą, pvz: ./main -sha256 failas.txt
## Hash funkcijos pseudo-kodas, pavaizduotas blokine schema:
![hashAlgoritmas drawio](https://user-images.githubusercontent.com/78591148/134543128-0ed81716-9f27-4451-a691-5717b81d12cf.png)
## Eksperimentinė tyrimo-analizė:
### Tiek tuščio failo, tiek failo su 10000 simbolių hash'avimas duoda rezultatą:
![image](https://user-images.githubusercontent.com/78591148/134548797-71ee9d5d-e661-4fba-b009-3bb82bdcbbac.png)
### Hash funkcijos output'o ilgis nepriklauso nuo input'o dydžio (empty.txt - 0 simbolių, 1g.txt ir 1a.txt - po vieną simbolį, random1.txt ir random2.txt - po 10000 simbolių):
![image](https://user-images.githubusercontent.com/78591148/134551842-fad009d8-c0c1-4ae0-bf8c-19973f95eb36.png)
### Deterministiškumo tyrimas, hash'uojant tą patį failą iš 10000 simbolių:
![image](https://user-images.githubusercontent.com/78591148/134550977-d110e2f9-b724-409e-b40e-97a14f1208c1.png)
## Konstitucijos hash'avimas:
### Konstitucijos hash'avimas po eilutę, naudojant mano hash funkciją: 
![image](https://user-images.githubusercontent.com/78591148/134553160-d12c6a24-bae5-4c5a-b081-3f2f51f2d725.png)
### Mano hash funkcijos lyginimas su md5, hash'uojant konstituciją po eilutę:
![image](https://user-images.githubusercontent.com/78591148/134553504-682c606f-6e51-4617-9d82-29d033d9d529.png)
### Su sha256:
![image](https://user-images.githubusercontent.com/78591148/134553575-bc29fba7-4bdb-4196-ac53-402452eb53be.png)
## Atsparumo kolizijai tyrimas:
### Rezultatai su 100000 simbolių eilučių porų, lyginant ar kiekvienos poros hash'ai nesutampa:
![image](https://user-images.githubusercontent.com/78591148/134558642-587ee547-c2d9-4bc7-9fae-fa126f9f285b.png)
## Lavinos efekto tyrimas:
### Rezultatai su 100000 simbolių eilučių porų, kai kiekviena pora viena nuo kitos skiriasi tik vienu simboliu:
![image](https://user-images.githubusercontent.com/78591148/134558977-41a456bb-4ab4-470f-947a-b4c860e32ce6.png)
## Išvados:
### Stiprybės - aukštas procentinis skirtingumas hex'ų lygmenyje (apie 92%) bei bit'ų lygmenyje (apie 42%).
### Trūkumai - algoritmas lėčiau veikia nei md5 ar sha256.
