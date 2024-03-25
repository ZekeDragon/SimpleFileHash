<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="sv_SE">
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="aboutdialog.ui" line="14"/>
        <source>About SimpleFileHash</source>
        <translation>Om SimpleFileHash</translation>
    </message>
    <message>
        <location filename="aboutdialog.ui" line="25"/>
        <source>The Simple Qt File Hashing Application</source>
        <translation>Den Enkla Qt-filhashningsapplikationen</translation>
    </message>
    <message>
        <location filename="aboutdialog.ui" line="35"/>
        <source>Alpha Version 0.1.0</source>
        <translation>Alfaversion 0.1.0</translation>
    </message>
    <message>
        <location filename="aboutdialog.ui" line="45"/>
        <source>AKA &quot;ZekeDragon&quot; or Ezekiel Oruven</source>
        <translation>Känd som&quot;ZekeDragon&quot; eller Ezekiel Oruven</translation>
    </message>
    <message>
        <location filename="aboutdialog.ui" line="48"/>
        <source>Copyright ©2024 Quinn Kirwan</source>
        <translation>Upphovsrätt ©2024 Quinn Kirwan</translation>
    </message>
    <message>
        <location filename="aboutdialog.ui" line="64"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This is a simple application that allows you to create cryptographic hashes of files, save those hashes to a hash sum file, compare those hashes with existing hash sum files, or compare two sets of files&apos; hashes.&lt;/p&gt;&lt;p&gt;You are free to use this program under the terms of the GNU Affero General Public License, Version 3. You should be able to find a copy of this license along with the source code or distribution of this application. If not, please find the license with the source code on the project&apos;s GitHub page: &lt;a href=&quot;https://github.com/ZekeDragon/SimpleFileHash&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#8ab4f8;&quot;&gt;https://github.com/ZekeDragon/SimpleFileHash&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Det här är en enkel applikation som låter dig skapa kryptografiska hashar av filer, spara dessa hashar till en hashsummafil, jämföra dessa hashar med befintliga hashsummafiler eller jämföra två uppsättningar filers hashar.&lt;/p&gt;&lt;p&gt;Du är fri att använda detta program under villkoren i GNU Affero General Public License, Version 3. Du borde kunna hitta en kopia av denna licens tillsammans med källkoden eller distributionen av denna applikation. Om inte, vänligen hitta licensen med källkoden på projektets GitHub-sida: &lt;a href=&quot;https://github.com/ZekeDragon/SimpleFileHash&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#8ab4f8;&quot;&gt;https://github.com/ZekeDragon/SimpleFileHash&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="aboutdialog.ui" line="77"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;a href=&quot;https://icons8.com/icon/13598/hash&quot;&gt;Hash&lt;/a&gt; icon by &lt;a href=&quot;https://icons8.com&quot;&gt;Icons8&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation></translation>
    </message>
    <message>
        <location filename="aboutdialog.ui" line="84"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;a href=&quot;https://www.flaticon.com/free-icons/cancel&quot; title=&quot;cancel icons&quot;&gt;Cancel icons created by Alfredo Hernandez - Flaticon&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation></translation>
    </message>
    <message>
        <location filename="aboutdialog.ui" line="91"/>
        <source>Click here to close this window.</source>
        <translation>Klicka här för att stänga detta fönster.</translation>
    </message>
</context>
<context>
    <name>HashMatchWindow</name>
    <message>
        <location filename="hashmatchwindow.ui" line="14"/>
        <source>Matching Window</source>
        <translation>Matchningsfönster</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.ui" line="30"/>
        <source>Add more hashes that are considered a &quot;match&quot; below (each line is a new hash):</source>
        <translation>Lägg till fler matchande hashar nedan (varje rad är en ny hash):</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.ui" line="37"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Enter hashes (in hexadecimal) you want to match to in this box. Each line represents a new hash.&lt;/p&gt;&lt;p&gt;This box can accept hashes in uppercase or lowercase, it will automatically convert as appropriate.&lt;/p&gt;&lt;p&gt;You may optionally include a filename after the hash by inputting a space followed by the filename, and optionally after a filename may include a hash algorithm name.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Ange hashar (i hexadecimal) du vill matcha i detta fält. Varje rad representerar en ny hash.&lt;/p&gt;&lt;p&gt;Detta fält kan acceptera hashar med versaler eller gemener, det kommer att konvertera dem automatiskt vid behov.&lt;/p&gt;&lt;p&gt;Du kan valfritt inkludera ett filnamn efter hashtaget genom att mata in ett mellanslag följt av filnamnet, och valfritt efter ett filnamn kan du inkludera ett namn på hashalgoritm.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.ui" line="46"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Press this button to add the hashes input above to the matching table below. Any optionally input filename and hash algorithm name will be placed accordingly, or a &lt;span style=&quot; font-weight:700;&quot;&gt;*&lt;/span&gt; symbol will appear if no filename or algorithm are provided.&lt;/p&gt;&lt;p&gt;The &lt;span style=&quot; font-weight:700;&quot;&gt;*&lt;/span&gt; symbol means that any filename or algorithm will be matched to. This is unlikely to be a problem with algorithms, but filename may be an issue if you have multiple files that may be the same but have different names.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Tryck på den här knappen för att lägga till de ovan angivna hasharna i matchningstabellen nedan. Eventuellt angivet filnamn och hashalgoritmnamn placeras därefter, eller ett &lt;span style=&quot; font-weight:700;&quot;&gt;*&lt;/span&gt; -symbol visas om inget filnamn eller algoritm tillhandahålls.&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-weight:700;&quot;&gt;*&lt;/span&gt; -symbolen innebär att alla filnamn eller algoritmer kommer att matchas. Detta är osannolikt att vara ett problem med algoritmer, men filnamnet kan vara ett problem om du har flera filer som kan vara lika men har olika namn.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.ui" line="49"/>
        <source>Add Hash Matches</source>
        <translation>Lägg till Hashar</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.ui" line="52"/>
        <source>Ctrl+M</source>
        <translation></translation>
    </message>
    <message>
        <location filename="hashmatchwindow.ui" line="59"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Clear the text entry field above without adding those hashes to the matching table below. Primarily useful if there is a mistake in the input.&lt;/p&gt;&lt;p&gt;WARNING: You will lose any data entered above if it is not stored or saved elsewhere!&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Rensa textinmatningsfältet ovan utan att lägga till dessa hashar i matchningstabellen nedan. Främst användbart om det finns ett fel i inmatningen.&lt;/p&gt;&lt;p&gt;VARNING: Du kommer att förlora alla data som matats in ovan om den inte lagras eller sparas någon annanstans!&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.ui" line="62"/>
        <source>Clear Hash Entry</source>
        <translation>Rensa inmatningsfältet</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.ui" line="65"/>
        <source>Ctrl+R</source>
        <translation></translation>
    </message>
    <message>
        <location filename="hashmatchwindow.ui" line="72"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Press this to clear the Matches Found and Unmatched Hashes tables below.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Tryck här för att rensa tabellerna Hittade Matchningar och Omatchade Hashar nedan.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.ui" line="75"/>
        <source>Clear Hash Matches</source>
        <translation>Rensa matchningsfältet</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.ui" line="78"/>
        <source>Ctrl+Shift+R</source>
        <translation>Ctrl+Skift+R</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.ui" line="95"/>
        <source>0/0 Matches Found</source>
        <translation>0/0 Hittade Matchningar</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.ui" line="145"/>
        <source>0 Unmatched Hashes</source>
        <translation>0 Omatchade Hashar</translation>
    </message>
    <message>
        <source>Matches Found:</source>
        <translation type="vanished">Matchningar hittade:</translation>
    </message>
    <message>
        <source>Save hash matches between successive hash jobs?</source>
        <translation type="vanished">Spara matchningar mellan på varandra följande hash-jobb?</translation>
    </message>
</context>
<context>
    <name>KirHut::SFH::HashMatchModel</name>
    <message>
        <location filename="hashmatchmodel.cpp" line="59"/>
        <source>This is the &quot;hash function&quot; that your hash file used to generate this matching hash. The hashed string should match the type of the hash and the hash string itself.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="64"/>
        <source>This is the name of the file that is expected to match with this hash. There is no match if the file name of the hashed file is different than the one that is expected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="68"/>
        <source>This is one of the correct hashes you have provided to the application, either through a file like a hash sum file or directly through the input above. When the hashing application encounters a file that, after hashing, has an identical string (and hash function if applicable), then this field will appear green. If there is no matches after all of the hashes have been completed, they will be highlighted in red.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="116"/>
        <source>*</source>
        <comment>Should be a symbol that means &quot;all algorithms&quot; that is very short.</comment>
        <translation></translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="125"/>
        <source>*</source>
        <comment>Should be a symbol that means &quot;all file names&quot; that is very short.</comment>
        <translation></translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="152"/>
        <source>This matches with any algorithm that generates this hash.</source>
        <translation>Detta matchar med vilken algoritm som helst som genererar denna hash.</translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="161"/>
        <source>This matches with any file name that generates this hash.</source>
        <translation>Detta matchar med vilket filnamn som helst som genererar denna hash.</translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="167"/>
        <source>Matched!</source>
        <translation>Matchat!</translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="167"/>
        <source>Not matched.</source>
        <translation>Inte matchat.</translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="203"/>
        <location filename="unmatchedhashmodel.cpp" line="129"/>
        <source>Algorithm</source>
        <translation>Algoritm</translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="203"/>
        <location filename="unmatchedhashmodel.cpp" line="130"/>
        <source>File Name</source>
        <translation>Filnamn</translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="203"/>
        <location filename="unmatchedhashmodel.cpp" line="131"/>
        <source>Hash String</source>
        <translation>Hash-text</translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="206"/>
        <source>The algorithm each found hash is matched to.</source>
        <translation>Algoritmen som varje hittad hash är matchad med.</translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="207"/>
        <source>The filename expected for the found hash.</source>
        <translation>Förväntat filnamn för den hittade hashen.</translation>
    </message>
    <message>
        <location filename="hashmatchmodel.cpp" line="208"/>
        <source>The hash that is being searched for.</source>
        <translation>Hashen som söks efter.</translation>
    </message>
</context>
<context>
    <name>KirHut::SFH::HashMatchWindow</name>
    <message>
        <location filename="hashmatchwindow.cpp" line="67"/>
        <source>Save hash matches between successive hash jobs?</source>
        <translation>Spara matchningar mellan på varandra följande hash-jobb?</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.cpp" line="68"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;By default, each time you start a new hash job, it will reset all of the matches below. If you want it to keep the matches found below, check this box.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Som standard, varje gång du startar ett nytt hash-jobb, återställs alla matchningar nedan. Om du vill behålla de matchningar som hittats nedan, markera den här rutan.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.cpp" line="223"/>
        <source>Failed to Open HashDeep File</source>
        <translation>Misslyckades med att öppna HashDeep-fil</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.cpp" line="224"/>
        <source>The provided HashDeep file was formatted incorrectly.</source>
        <translation>Den angivna HashDeep-filen var formaterad felaktigt.</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.cpp" line="235"/>
        <source>Failed to Open Hash Sum File</source>
        <translation>Misslyckades med att öppna hash-summafilen</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.cpp" line="236"/>
        <source>The provided hash sum file was formatted incorrectly.</source>
        <translation>Den angivna hash-summafilen var formaterad felaktigt.</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.cpp" line="242"/>
        <source>File Opening Failed</source>
        <translation>Misslyckades med att öppna filen</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.cpp" line="243"/>
        <source>Attempting to open the file %1 for read-only failed for an unknown reason.</source>
        <translation>Försökte öppna filen %1 för endast läsning, men misslyckades av okänd anledning.</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.cpp" line="249"/>
        <source>File Not Found</source>
        <translation>Fil hittades inte</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.cpp" line="250"/>
        <source>The file passed to the Hash Matching Window does not exist: %1</source>
        <translation>Filen som skickades till hashmatchningsfönstret finns inte: %1</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.cpp" line="349"/>
        <source>%1/%2 Matches Found</source>
        <translation>%1/%2 Hittade Matchningar</translation>
    </message>
    <message>
        <location filename="hashmatchwindow.cpp" line="354"/>
        <source>%1 Unmatched Hashes</source>
        <translation>%1 Omatchade Hashar</translation>
    </message>
</context>
<context>
    <name>KirHut::SFH::HashTask</name>
    <message>
        <location filename="hashalgo.cpp" line="170"/>
        <source>Message Digest 5</source>
        <translation>Meddelandedigest 5</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="171"/>
        <source>Secure Hash Algorithm 1</source>
        <translation>Säkert Hashalgoritm 1</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="172"/>
        <source>224-bit Secure Hash Algorithm 2</source>
        <translation>224-bit Säkert Hashalgoritm 2</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="173"/>
        <source>256-bit Secure Hash Algorithm 2</source>
        <translation>256-bit Säkert Hashalgoritm 2</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="174"/>
        <source>384-bit Secure Hash Algorithm 2</source>
        <translation>384-bit Säkert Hashalgoritm 2</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="175"/>
        <source>512-bit Secure Hash Algorithm 2</source>
        <translation>512-bit Säkert Hashalgoritm 2</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="176"/>
        <source>224-bit Secure Hash Algorithm 3</source>
        <translation>224-bit Säkert Hashalgoritm 3</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="177"/>
        <source>256-bit Secure Hash Algorithm 3</source>
        <translation>256-bit Säkert Hashalgoritm 3</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="178"/>
        <source>384-bit Secure Hash Algorithm 3</source>
        <translation>384-bit Säkert Hashalgoritm 3</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="179"/>
        <source>512-bit Secure Hash Algorithm 3</source>
        <translation>512-bit Säkert Hashalgoritm 3</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="180"/>
        <source>BLAKE2s Hash</source>
        <translation>Blake2s-hash</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="181"/>
        <source>BLAKE2b Hash</source>
        <translation>Blake2b-hash</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="182"/>
        <source>Tiger Hash</source>
        <translation>Tiger-hash</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="183"/>
        <source>Whirlpool Hash</source>
        <translation>Whirlpool-hash</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="184"/>
        <source>Message Digest 4</source>
        <translation>Meddelandedigest 4</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="185"/>
        <source>Message Digest 2</source>
        <translation>Meddelandedigest 2</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="186"/>
        <source>128-bit Message Digest 6</source>
        <translation>128-bit Meddelandedigest 6</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="187"/>
        <source>224-bit Message Digest 6</source>
        <translation>224-bit Meddelandedigest 6</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="188"/>
        <source>256-bit Message Digest 6</source>
        <translation>256-bit Meddelandedigest 6</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="189"/>
        <source>384-bit Message Digest 6</source>
        <translation>384-bit Meddelandedigest 6</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="190"/>
        <source>512-bit Message Digest 6</source>
        <translation>512-bit Meddelandedigest 6</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="191"/>
        <source>RIPE Message Digest-160</source>
        <translation>RIPE Meddelanderesumé-160</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="192"/>
        <source>RIPE Message Digest-256</source>
        <translation>RIPE Meddelanderesumé-256</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="193"/>
        <source>ShangMi 3 Hash</source>
        <translation>ShangMi 3-hash</translation>
    </message>
    <message>
        <location filename="hashalgo.cpp" line="194"/>
        <source>None</source>
        <comment>The algorithm type &apos;None&apos;</comment>
        <translation>Ingen</translation>
    </message>
    <message>
        <location filename="hashtask.cpp" line="331"/>
        <source>The file path provided does not exist.</source>
        <translation>Den angivna filvägen finns inte.</translation>
    </message>
    <message>
        <location filename="hashtask.cpp" line="335"/>
        <source>The file provided is not a regular file.</source>
        <translation>Den angivna filen är inte en vanlig fil.</translation>
    </message>
    <message>
        <location filename="hashtask.cpp" line="348"/>
        <source>Invalid Hash Function: </source>
        <translation>Ogiltig hashfunktion: </translation>
    </message>
    <message>
        <source>Received Crypto++ file %1 from attempt to read file: </source>
        <translation type="vanished">Mottog Crypto++ fil %1 från försök att läsa fil: </translation>
    </message>
    <message>
        <source>This is due to an underlying Input error.</source>
        <translation type="vanished">Detta beror på en underliggande inmatningsfel.</translation>
    </message>
    <message>
        <source>This is because a data buffer cannot be flushed correctly.</source>
        <translation type="vanished">Detta beror på att en data buffert inte kan spolas korrekt.</translation>
    </message>
    <message>
        <source>There was a data integrity check that failed.</source>
        <translation type="vanished">Det fanns en kontroll av dataintegritet som misslyckades.</translation>
    </message>
    <message>
        <source>This is because an argument provided was invalid.</source>
        <translation type="vanished">Detta beror på att ett angivet argument var ogiltigt.</translation>
    </message>
    <message>
        <source>This is because the data was in an invalid format.</source>
        <translation type="vanished">Detta beror på att data var i ett ogiltigt format.</translation>
    </message>
    <message>
        <source>This is reportedly because the functionality is not implemented.</source>
        <translation type="vanished">Detta rapporteras bero på att funktionaliteten inte är implementerad.</translation>
    </message>
    <message>
        <source>This is because of an unknown error within Crypto++.</source>
        <translation type="vanished">Detta beror på ett okänt fel inom Crypto++.</translation>
    </message>
    <message>
        <source>The error reports: </source>
        <translation type="vanished">Felrapporten: </translation>
    </message>
    <message>
        <source>error</source>
        <translation type="vanished">fel</translation>
    </message>
    <message>
        <location filename="hashtask.cpp" line="393"/>
        <source>Unknown error from Crypto++</source>
        <translation>Okänt fel från Crypto++</translation>
    </message>
    <message>
        <source>read error</source>
        <translation type="vanished">läs fel</translation>
    </message>
    <message>
        <location filename="hashtask.cpp" line="397"/>
        <source>Could not read the provided file</source>
        <translation>Kunde inte läsa den angivna filen</translation>
    </message>
    <message>
        <source>open error</source>
        <translation type="vanished">öppna fel</translation>
    </message>
    <message>
        <location filename="hashtask.cpp" line="401"/>
        <source>Failed to open the file provided</source>
        <translation>Det gick inte att öppna den angivna filen</translation>
    </message>
    <message>
        <source>There was an error in the filesystem while accessing this file: </source>
        <translation type="vanished">Ett fel uppstod i filsystemet när denna fil skulle nås: </translation>
    </message>
    <message>
        <source>The reported error is: </source>
        <translation type="vanished">Det rapporterade felet är: </translation>
    </message>
    <message>
        <location filename="hashtask.cpp" line="407"/>
        <source>Filesystem interaction error</source>
        <translation>Fel vid filsysteminteraktion</translation>
    </message>
    <message>
        <source>The process ran out of memory while performing the hash task.</source>
        <translation type="vanished">Processen tog slut på minne när den utförde hash-uppgiften.</translation>
    </message>
    <message>
        <source>Filename in progress: </source>
        <translation type="vanished">Filnamn under bearbetning: </translation>
    </message>
    <message>
        <location filename="hashtask.cpp" line="412"/>
        <source>Hashing process ran out of memory</source>
        <translation>Hashningsprocessen tog slut på minne</translation>
    </message>
    <message>
        <source>An unknown exception was thrown from the running hash task.</source>
        <translation type="vanished">Ett okänt undantag kastades från den pågående hash-uppgiften.</translation>
    </message>
    <message>
        <location filename="hashtask.cpp" line="417"/>
        <source>An unknown error occurred during hash</source>
        <translation>Ett okänt fel inträffade under hashningen</translation>
    </message>
    <message>
        <location filename="hashtask.cpp" line="434"/>
        <location filename="hashtask.cpp" line="541"/>
        <source>Canceled!</source>
        <translation>Avbruten!</translation>
    </message>
    <message>
        <location filename="hashtask.cpp" line="436"/>
        <source>Calculating Hash...</source>
        <translation>Beräknar hash...</translation>
    </message>
    <message>
        <location filename="hashtask.cpp" line="444"/>
        <source>Hash has not started</source>
        <translation>Hashen har inte startat</translation>
    </message>
</context>
<context>
    <name>KirHut::SFH::HashTasksModel</name>
    <message>
        <location filename="hashtasksmodel.cpp" line="126"/>
        <source>The name of the file being hashed.</source>
        <translation>Namnet på filen som hashas.</translation>
    </message>
    <message>
        <location filename="hashtasksmodel.cpp" line="130"/>
        <source>The hashing function in use.</source>
        <translation>Hashfunktionen som används.</translation>
    </message>
    <message>
        <location filename="hashtasksmodel.cpp" line="134"/>
        <source>Copy this or use the Hash Match Window to compare with the known hashes.</source>
        <translation>Kopiera detta eller använd Hash Match-fönstret för att jämföra med de kända hasharna.</translation>
    </message>
    <message>
        <location filename="hashtasksmodel.cpp" line="137"/>
        <source>The status of the hashing job.</source>
        <translation>Status för hashningsjobbet.</translation>
    </message>
    <message>
        <location filename="hashtasksmodel.cpp" line="142"/>
        <source>This is where the &quot;file name&quot; of the file you are making a hash of is displayed. This does not include the rest of the file path, but should have the full name on display. You can compare your known hashes to the ones generated using these names.</source>
        <translation>Här visas &quot;filnamnet&quot; på filen du skapar en hash av. Detta inkluderar inte resten av filens sökväg, men bör visa hela namnet. Du kan jämföra dina kända hashar med de som genererats med dessa namn.</translation>
    </message>
    <message>
        <location filename="hashtasksmodel.cpp" line="148"/>
        <source>This is the &quot;hash function&quot; that is being used by this application to hash your file. A hash function is a long series of math that is intended to uniquely identify your file from all other files in a manner that prevents any other file from pretending they are your file. File hashing is a fundamental part of computer security.</source>
        <translation>Det här är &quot;hashfunktionen&quot; som används av den här applikationen för att skapa en hash av din fil. En hashfunktion är en lång serie matematiska operationer avsedd att unikt identifiera din fil från alla andra filer på ett sätt som förhindrar att någon annan fil låtsas vara din fil. Filhashning är en grundläggande del av datorsäkerhet.</translation>
    </message>
    <message>
        <location filename="hashtasksmodel.cpp" line="154"/>
        <source>This is where the completed hash is displayed when it is done, and the status of the hash job when it is not. There will be a loading bar for each hashing operation that will update regularly, so with long-running hashes you can watch the progress. This software is designed to be multi-threaded, so multiple bars should be going at the same time.</source>
        <translation>Det här är där den färdiga hashen visas när den är klar, och statusen för hasjobbet när den inte är det. Det kommer att finnas en laddningsindikator för varje hashoperation som uppdateras regelbundet, så med långvariga hashar kan du följa framsteg. Den här programvaran är utformad för att vara flertrådad, så flera indikatorer ska vara igång samtidigt.</translation>
    </message>
    <message>
        <location filename="hashtasksmodel.cpp" line="188"/>
        <source>Name of File</source>
        <translation>Filnamn</translation>
    </message>
    <message>
        <location filename="hashtasksmodel.cpp" line="188"/>
        <source>Algorithm</source>
        <translation>Algoritm</translation>
    </message>
    <message>
        <location filename="hashtasksmodel.cpp" line="188"/>
        <source>Hash Function Result</source>
        <translation>Hashfunktionsresultat</translation>
    </message>
</context>
<context>
    <name>KirHut::SFH::MainWindow</name>
    <message>
        <source>The Qt Simple File Hashing Application</source>
        <translation type="vanished">Qt Enkel Filhashningsapplikation</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="86"/>
        <source>Select Folder...</source>
        <translation>Välj mapp...</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="211"/>
        <source>Start Hashing!</source>
        <translation>Starta hashning!</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="215"/>
        <source>Cancel Hashing!</source>
        <translation>Avbryt hashning!</translation>
    </message>
    <message>
        <source>Copy Hashes!</source>
        <translation type="vanished">Kopiera hashar!</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="219"/>
        <source>Match Hashes!</source>
        <translation>Matcha Hashar!</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="331"/>
        <source>Select one or more files to Hash</source>
        <translation>Välj en eller flera filer att hasha</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="344"/>
        <source>folder</source>
        <translation>mapp</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="348"/>
        <source>directory</source>
        <translation>katalog</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="351"/>
        <source>Select a %1 to Hash</source>
        <translation>Välj en %1 att hasha</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="391"/>
        <source>Select one hash sum or HashDeep file</source>
        <translation>Välj en hashsumma eller HashDeep-fil</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="415"/>
        <source>https://www.kirhut.com/docs/doku.php?id=monthly:project1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="filehashapplication.cpp" line="110"/>
        <source>System Default (English)</source>
        <translation>Systemstandard (svenska)</translation>
    </message>
    <message>
        <location filename="filehashapplication.cpp" line="125"/>
        <source>The Simple Qt File Hashing Application</source>
        <translation>Den Enkla Qt-filhashningsapplikationen</translation>
    </message>
</context>
<context>
    <name>KirHut::SFH::PreferencesDialog</name>
    <message>
        <source>System Default</source>
        <translation type="vanished">Systemets standard</translation>
    </message>
</context>
<context>
    <name>KirHut::SFH::UnmatchedHashModel</name>
    <message>
        <location filename="unmatchedhashmodel.cpp" line="53"/>
        <source>The hash function algorithm that generated this hash.</source>
        <translation>Hashfunktionsalgoritmen som genererade denna hash.</translation>
    </message>
    <message>
        <location filename="unmatchedhashmodel.cpp" line="54"/>
        <source>The filename of the file that this hash was generated from.</source>
        <translation>Filnamnet på filen som denna hash genererades från.</translation>
    </message>
    <message>
        <location filename="unmatchedhashmodel.cpp" line="55"/>
        <source>This is a hash that hasn&apos;t been matched with any expected hash.</source>
        <translation>Detta är en hash som inte har matchats med någon förväntad hash.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.ui" line="14"/>
        <source>Hashing Window</source>
        <oldsource>MainWindow</oldsource>
        <translation>Hashningsfönster</translation>
    </message>
    <message>
        <source>File Hashing Options</source>
        <translation type="vanished">Alternativ för filhashning</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="23"/>
        <source>File Selection (you may also drag and drop):</source>
        <translation>Filval (du kan också dra och släppa):</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="30"/>
        <source>Select files to hash.</source>
        <translation>Välj filer att skapa hash för.</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="33"/>
        <source>Select Files...</source>
        <translation>Välj filer...</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="40"/>
        <source>Select a directory to hash.</source>
        <translation>Välj en katalog att skapa hash för.</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="43"/>
        <source>Select Directory...</source>
        <translation>Välj katalog...</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="54"/>
        <source>Hashing Algorithm: </source>
        <translation>Hashningsalgoritm: </translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="67"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Select a hashing function algorithm using this drop-down menu. This will be the default selected algorithm for files opened with the above buttons or dropped on this window when there are multiple files.&lt;/p&gt;&lt;p&gt;If there is a single file only, this window will hash that file using all of the selected algorithms in the Preferences menu.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="83"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;You cannot press this until you have selected files to hash.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="86"/>
        <source>Start Hashing!</source>
        <translation>Starta hashning!</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="118"/>
        <source>Simple&amp;Hash</source>
        <translation>Enkel &amp;Hash</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="127"/>
        <source>&amp;Matching</source>
        <translation>&amp;Matchning</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="131"/>
        <source>&amp;Match Against</source>
        <translation>&amp;Matcha mot</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="142"/>
        <source>&amp;Help</source>
        <translation>&amp;Hjälp</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="154"/>
        <source>&amp;Hash Files...</source>
        <translation>&amp;Hasha filer...</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="192"/>
        <source>Match &amp;Folders</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="197"/>
        <source>&amp;About SimpleFileHash</source>
        <translation>&amp;Om SimpleFileHash</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="202"/>
        <source>&amp;Online Help</source>
        <translation>&amp;Onlinehjälp</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="207"/>
        <source>&amp;Current Hashes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Hash Files</source>
        <translation type="vanished">&amp;Hasha filer</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="159"/>
        <source>&amp;Preferences</source>
        <translation>&amp;Inställningar</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="164"/>
        <source>E&amp;xit</source>
        <translation>&amp;Avsluta</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="167"/>
        <source>Ctrl+Q</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="172"/>
        <source>&amp;Copy All Hashes</source>
        <translation>&amp;Kopiera alla hashar</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="177"/>
        <source>&amp;Input</source>
        <translation>&amp;Inmatning</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="182"/>
        <source>&amp;Hashdeep File</source>
        <translation>&amp;HashDeep-fil</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="187"/>
        <source>Hash&amp;sum File</source>
        <translation>&amp;HashSum-fil</translation>
    </message>
</context>
<context>
    <name>PreferencesDialog</name>
    <message>
        <location filename="preferencesdialog.ui" line="14"/>
        <source>User Preferences</source>
        <translation>Användarinställningar</translation>
    </message>
    <message>
        <location filename="preferencesdialog.ui" line="27"/>
        <source>Basic Options</source>
        <translation>Grundalternativ</translation>
    </message>
    <message>
        <location filename="preferencesdialog.ui" line="33"/>
        <source>Go Through Subdirectories When Hashing a Directory?</source>
        <oldsource>Go Through Subdirectories When Hashing a Directory.</oldsource>
        <translation>Gå igenom Underkataloger När Du Skapar en Katalog?</translation>
    </message>
    <message>
        <location filename="preferencesdialog.ui" line="40"/>
        <source>Display Generated Hashes in Uppercase?</source>
        <translation>Visa Genererade Hashar med Stora Bokstäver?</translation>
    </message>
    <message>
        <location filename="preferencesdialog.ui" line="49"/>
        <source>Override System Theme:</source>
        <translation>Åsidosätt systemtema:</translation>
    </message>
    <message>
        <location filename="preferencesdialog.ui" line="56"/>
        <source>Light Mode</source>
        <translation>Ljust läge</translation>
    </message>
    <message>
        <location filename="preferencesdialog.ui" line="63"/>
        <source>Dark Mode</source>
        <translation>Mörkt läge</translation>
    </message>
    <message>
        <location filename="preferencesdialog.ui" line="80"/>
        <source>Set Limit on Number of FIles:</source>
        <translation>Ange begränsning för antal filer:</translation>
    </message>
    <message>
        <location filename="preferencesdialog.ui" line="139"/>
        <source>Install Context Menu Options</source>
        <translation>Installera alternativ i snabbmenyn</translation>
    </message>
    <message>
        <location filename="preferencesdialog.ui" line="176"/>
        <source>Single File Algos</source>
        <translation>Algoritmer för En Fil</translation>
    </message>
    <message>
        <location filename="preferencesdialog.ui" line="98"/>
        <source>Default Algorithm:</source>
        <translation>Standardalgoritm:</translation>
    </message>
    <message>
        <location filename="preferencesdialog.ui" line="119"/>
        <source>Select Language:</source>
        <translation>Välj Språk:</translation>
    </message>
</context>
</TS>
