String putReport(const String& var)
{
//swap_to_usb();
  if(var == "rows") {
        char report1[1200] = {0}; //12 x 100
        char report2[1200] = {0}; //12 x 100
        char temp[100] = {0}; //4 floats of 9 and one of 8 = 45 and 35 html 
        float econ_lt;
        float econ_ht;
        float eret_lt;
        float eret_ht;
        float mgas;
        int next;
        int prev;
        int maand = month()+1;
        // the monthly values are written at the very beginning of each month so month2 is end month1
        // if the nextmonth = smaller, this is a value of a former year
        // we can not calculate it unless x - the current month, thec we can calculate the todate value
        
        // we have 12 values 1 to 12
        //for mont 1 the value = 2 -/- 1 and 11 = 12 -/-11 and 12 = 1-12
        //the struct goes fom 0 to 11
        
        for(int x=1; x<13; x++) {
       
        econ_lt = 0;
        econ_ht = 0;
        eret_lt = 0;
        eret_ht = 0;
        mgas = 0;       
//        
        next = x+1;
        if(next == 13) next = 0; // the last iteration is thus 0 - 11   
//        prev = x-1;
//        if(prev == 0) prev = 12; 
         Serial.println("x = " + String(x));
        //if there are no values we skip this whole iteration
        if ( MVALS[x].EC_LT != 0 || MVALS[x].EC_HT != 0 || MVALS[x].ER_LT != 0 || MVALS[x].ER_HT != 0 || MVALS[x].mGAS != 0 )
        { 
            Serial.println("found values in month " + String(x));
            // we are in the month 8 and have values
            //check if there are values in the next month and they are greater
            // if we have greater values of the next month, we could calculate
            if( MVALS[next].EC_LT != 0 &&  MVALS[next].EC_LT > MVALS[x].EC_LT ) { 
            Serial.println("also values in next month " + String(next));
                   econ_lt = MVALS[next].EC_LT - MVALS[x].EC_LT;
                   // this would be over the current month
            } else {
            // we have values in x but not (greater) values in next
            // we can check if the current values are greater than the ones in the file
            // we can calculate a todate value that would be over the current mont 
            Serial.println("no values in next month so calculate todate"); 
               if (ECON_LT > MVALS[x].EC_LT ) {
                 econ_lt = ECON_LT - MVALS[x].EC_LT; //
               } else { 
                Serial.println("could not calculate todate for " + String(x));}
            }  
          
            if( MVALS[next].EC_HT != 0 &&  MVALS[next].EC_HT > MVALS[x].EC_HT ) { 
               econ_ht = MVALS[next].EC_HT - MVALS[x].EC_HT;
            } else {
               if (ECON_HT > MVALS[x].EC_HT ) econ_ht = ECON_HT - MVALS[x].EC_HT; //
            }            

            if( MVALS[next].ER_LT != 0 &&  MVALS[next].ER_LT > MVALS[x].ER_LT ) { 
               eret_lt = MVALS[next].ER_LT - MVALS[x].ER_LT;
            } else {
               if (ERET_LT > MVALS[x].ER_LT ) eret_lt = ERET_LT - MVALS[x].ER_LT; //
            } 
            
            if( MVALS[next].ER_HT != 0 &&  MVALS[next].ER_HT > MVALS[x].ER_HT ) { 
               eret_ht = MVALS[next].ER_HT - MVALS[x].ER_HT;
            } else {
               if (ERET_HT > MVALS[x].ER_HT ) eret_ht = ERET_HT - MVALS[x].ER_HT; //
            }             


            if( MVALS[next].mGAS != 0 &&  MVALS[next].mGAS > MVALS[x].mGAS ) { 
               mgas = MVALS[next].mGAS - MVALS[x].mGAS;
            } else {
               if (mGAS > MVALS[x].mGAS ) mgas = mGAS - MVALS[x].mGAS; //
            }             
          
       } else { Serial.println("skipped line " + String(x));}    
        // for each month we print a line
        // we want the current month alwas be the last one
        sprintf(temp, "<tr id=\"row%.d\"><td>%.d<td>%.2f<td>%.2f<td>%.2f<td>%.2f<td>%.2f</td></tr>", x, x, econ_lt, econ_ht, eret_lt, eret_ht, mgas);  
        if( maand > x) {
            strcat(report2, temp); // so 9 10 11 12
        } else { 
            strcat(report1, temp);} //  1 - 8
        }    
        strcat(report1, report2); 
        //consoleLog(String(report));
        consoleLog("length = " + String(strlen(report2)));
        //swap_to_hw();
        return report1;
        }

return String();
}




void printStruct( String bestand, int what ) {
//input String bestand = "/Inv_Prop" + String(x) + ".str";
      //String bestand = bestand + String(i) + ".str"
      //readStruct(bestand); is done at boottime
        Serial.println("content of MVALS[" + String(what) + "]");
        Serial.println("EC_LT = " + String(MVALS[what].EC_LT));     
        Serial.println("EC_HT = " + String(MVALS[what].EC_HT));
        Serial.println("ER_LT = " + String(MVALS[what].ER_LT));
        Serial.println("ER_HT = " + String(MVALS[what].ER_HT));
        Serial.println("mGAS = " + String(MVALS[what].mGAS));

}
