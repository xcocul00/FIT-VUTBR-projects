%************************************************************************%
%ZAOSTRENIE OBRAZU
%************************************************************************%
original = imread('xcocul00.bmp');
obrazok = imread('xcocul00.bmp');
vystup = 'step1.bmp'       

matica_zaostrenia = [ -0.5 -0.5 -0.5; -0.5 5 -0.5; -0.5 -0.5 -0.5];
zaostreny = imfilter(obrazok, matica_zaostrenia);
imwrite(zaostreny, vystup); 

%************************************************************************%
%OTOCENIE OBRAZU
%************************************************************************%
vystup = 'step2.bmp';      
preklopeny = fliplr(zaostreny);   

imwrite(preklopeny, vystup);

%************************************************************************%
%MEDIANOVY FILTER
%************************************************************************%
vystup = 'step3.bmp';
median = medfilt2 (preklopeny, [5 5]);

imwrite(median,vystup);

%************************************************************************%
%ROZMAZANIE OBRAZU
%************************************************************************%
vystup = 'step4.bmp';
matica_rozmazania = [1 1 1 1 1; 1 3 3 3 1; 1 3 9 3 1; 1 3 3 3 1; 1 1 1 1 1] / 49;
rozmazany = imfilter(median,matica_rozmazania);

imwrite(rozmazany,vystup);
%imshow (vystup);

%************************************************************************%
%CHYBA
%************************************************************************%
porovnany = fliplr(rozmazany);
porovnany_double = im2double(porovnany);
original_double = im2double(original);
chyba =0;

velkost = size (original);
velkostX = min (velkost);
velkostY = max (velkost);

for (x=1: velkostX)
      for (y=1 : velkostY)
          chyba = chyba + abs (original_double(x,y) - porovnany_double (x,y));
      end;
end;

vysledok_chyba = (chyba/(velkostX * velkostY))*255

%************************************************************************%
%HISTOGRAM
%************************************************************************%
vystup = 'step5.bmp';
rozmazany_double = im2double (rozmazany);

minimum = min (rozmazany_double);
maximum = max (rozmazany_double);
vstup_L = min (minimum);
vstup_H = max (maximum); 
vystup_L = 0.0;
vystup_H = 1.0;

histogram = imadjust(rozmazany, [vstup_L vstup_H], [vystup_L vystup_H]);
imwrite(histogram, vystup);

%************************************************************************%
%ODCHYLKA
%************************************************************************%
rozmazany_double = im2double (rozmazany);
mean_no_hist = mean2 (rozmazany_double)*255
std_no_hist = std2 (rozmazany_double)*255
histogram_double = im2double (histogram);
mean_hist = mean2 (histogram_double)*255
std_hist = std2 (histogram_double)*255

%************************************************************************%
%ODCHYLKA
%************************************************************************%
vystup = 'step6.bmp';
N=2;
a=0;
b=255;

kvantizacia = zeros (velkostX, velkostY);
obrazok = double (histogram);
for (x=1 : velkostX)
    for (y=1 : velkostY)
        kvantizacia(x,y) = round(((2^N)-1)*(obrazok(x, y)-a)/(b-a))*(b-a)/((2^N)-1) + a;
    end;
end;
vysledok = uint8(kvantizacia);
imwrite(vysledok,vystup);
%************************************************************************%
%KONIEC
%************************************************************************%
