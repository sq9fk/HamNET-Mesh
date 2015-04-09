#!/bin/sh
# opuznienie uruchomienia skryptu w rc.local
#(sleep 30; /bin/aled.sh start) &
#
#wifi
#sleep 4 
inter=`iwinfo | grep "ESSID:" | cut -d " " -f1`
# odczyt Hardware
HardN=`iwinfo | grep " 0777:C002" | cut -d " " -f13`
HardM=`iwinfo | grep " 0777:" | cut -d " " -f13 | cut -d ":" -f1`
N=0
M=0
h=0  
P=1
B="\t BRAK STACJI AKUTALNIE POLACZONYCH  \n\n "
Ka=" Kierunek anteny - poziom SNR "
# ilosc odczytow 100
R=100
      if [ -z $HardN ]
        then 
        N=1
      fi
      if [ -z $HardM ]
         then
      M=1
       fi
     if [ $N = 1 -a $M = 1 ]
       then
      h=1
clear
echo -e "\n   TO NIE JEST UBIQUITI - NIE BEDZIE SYGNALIZACJI LED-em"
sleep 3
        fi
     if [ $h = 0 ]
         then
#
#         SIGNAL STRENGTH
# brak swiecenia  = SNR  < 396 brak sygnalu
# cigle swiecenie = SNR 350-390 bardzo slaby sygnal
# wolne blyski    = SNR 200-349 slaby sygnal
# srednie blyski =  SNR 150-199  dobry sygnal
# szybkie blyski =  SNR < 150    silny sygnal
#
clear
echo -e "\n--------------------------------------------------------------"
echo " Wskaznik sygnalu do ustawienia kierunku anteny V1.08  SP9FUT"
echo "      STACJE POLACZONE BEZPOSREDNIO W SIECI HamNet "
echo " `iwinfo $inter assoclist | grep "SNR"`"
echo "--------------------------------------------------------------"
#
       if [ $N = 0 ]
         then   
    if  [ $HardN = "0777:C002" ]
         then
echo -e "\t\t NanoStation2"
l1="/sys/class/leds/gpio1/trigger"
l2="/sys/class/leds/gpio1/delay_off"
l3="/sys/class/leds/gpio1/delay_on"
(echo "none" > $l1)
#
while [ $P -le $R ] ; do
#
# czas odczytu
sleep 1
export Wyn=`iwinfo $inter assoclist | grep "SNR" |cut -d " " -f 9 | cut -d ")" -f 1 | sort`
# pobranie SNR
Sa=$( echo $Wyn | cut -d " " -f1 )
      if [ $Sa > 0 ]
           then
        Snr=$Sa
       else
          P=$R
          Snr=1
         echo -e $B
      fi
SNR=$((400 -((Snr) ** 2 )))
   if [ $SNR -le 2 ]
         then
       SNR=2
   fi
   if [ $Snr -lt 0 ]
         then
    Snr=$((Snr * -1 ))
      fi 
   if [ $SNR -le 350 -a $Snr > 0 ]
      then
echo -e "$Ka $Snr  odczyt - $P"
# poziom sygnalu
(echo "timer" > $l1)
(echo "70" > $l2)
(echo "$SNR" >  $l3)
   elif [ $SNR -le 396 -a $Snr >0 ]
     then
# sygnal slaby
(echo "timer" > $l1)
(echo "70" > $l2)
(echo "3000" >  $l3)
     else
# brak sygnalu
(echo "none" > $l1)
      fi
   let P=$((P + 1 ))
     if [ $P -gt $R ]
        then
(echo "none" > $l1)
       fi
done
       fi
     fi
      if [ $M = 0 ]
       then
      if  [ $HardM = "0777" -a $N = 1 ]
         then
echo -e "\t\t UBIQUITI"

l4="/sys/class/leds/ubnt:orange:link2/trigger"
l5="/sys/class/leds/ubnt:orange:link2/delay_off"
l6="/sys/class/leds/ubnt:orange:link2/delay_on"
(echo "none" > $l4)
while [ $P -le $R ] ; do
# czas odczytu
sleep 1
export Wyn=`iwinfo $inter assoclist | grep "SNR" |cut -d " " -f 9 | cut -d ")" -f 1 | sort`
# pobranie SNR
Sa=$( echo $Wyn | cut -d " " -f1 )
      if [ $Sa > 0 ]
           then
        Snr=$Sa
       else
          P=$R
          Snr=1
         echo -e $B
      fi
SNR=$((400 -((Snr) ** 2 )))
   if [ $SNR -le 2 ]
         then
       SNR=2
   fi
   if [ $Snr -lt 0 ]
         then
    Snr=$((Snr * -1 ))
      fi 
   if [ $SNR -le 350 -a $Snr > 0 ]
      then
echo -e "$Ka $Snr  odczyt - $P"
# poziom sygnalu 
(echo "timer" > $l4)
(echo "70" > $l5)
(echo "$SNR" > $l6)
   elif [ $SNR -le 396 -a $Snr >0 ]
     then
# sygnal slaby
(echo "timer" > $l4)
(echo "70" > $l5)
(echo "3000" > $l6)
     else
# brak sygnalu
(echo "none" > $l4)
      fi
   let P=$((P + 1 ))
     if [ $P -gt $R ]
        then
(echo "none" > $l4)
       fi
done
       fi
     fi
   fi
 
# odczyt bez sygnalozacji
    if [ $h = 1 ]
         then
sleep 3
clear
echo -e "\n--------------------------------------------------------------"
echo " Wskaznik sygnalu do ustawienia kierunku anteny V1.08  SP9FUT"
echo "       STACJE POLACZONE BEZPOSREDNIO W SIECI HamNet "
echo " `iwinfo $inter assoclist | grep "SNR"`"
echo "--------------------------------------------------------------"
while [ $P -le $R ] ; do
sleep 2
export Wyn=`iwinfo $inter assoclist | grep "SNR" |cut -d " " -f 9 | cut -d ")" -f 1 | sort`
Sa=$( echo $Wyn | cut -d " " -f1 )
      if [ $Sa > 0 ]
           then
        Snr=$Sa
       else
          P=$R
          Snr=1
       echo -e $B
        fi
      if [ $Snr -lt 0 ]
         then
         Snr=$((Snr * -1 ))
      fi 

     if [ $Snr -gt 1 ]
         then
echo -e "$Ka $Snr  odczyt - $P"
     fi
     if [ $Snr > 0 ]
         then
      let P=$((P + 1 ))

     fi
done
     fi
# wersja z 2015.03.14