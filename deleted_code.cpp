// 这是特殊的Barrett而不是通用的，当仅当满足参数输入
// 这里的H足够大是因为，输入的模数q足够大，导致其余的结果都是0
// 测试通过的代码
void BarrettReductionSpeicail(BigNumber2048 A, BigNumber2048 q, BigNumber2048& r){
    // 计算 r = A (mod q)
    const int w_1 = 1024;
    const int w_2 = 1024;
    BigNumber2048 H("21cfe6e555cc899fe2825e8cc65797d4ebc06dea880b7306a07651d4e25edb0de534beba92372ed850dd420c21e502a67c0faf94ab48976b65726c2a8e18a3c0db601981daecad54effad29eac5dd2eb23d0b04a73d7fbaee800a9d23ae451377184db79b6fc3b17c5a81525792b46141fdd87cd4124cb777d0b1b64106dfc014"); // q_2 = H
    // H.print(); // H 读入正确
    const int q_1 = 1;
    H << 1; // q_3 = H << 1
    // H.print(); // H右移（也许）正确
    // BigNumber2048 H_2048 = H.upgrade();
    // BigNumber2048 A_2048 = A.upgrade();
    // r = A - q3 * q - q = A - H * q - q
    r = A - FNT::multiply(H, q).downgrade();
    if(r > (q<<1)){
        r = r - (q<<1);
    }
    else if(r > q){
        r = r - q;
    }
    r.print();
    
}

// 验证通过
void test_barret(){
    BigNumber1024 A("BB9F27A5DB4B4B9D260DCB04B56F6A7BE28602A982AC4A6994D272FB6D5C5E282BFE3704F123F74609C344918D9E7945AA832C6E03C80871B07CE46D638D34D7A0AD5B477B7A00B09DD712E57075E94D3006643C9AC14DAC3EDB355B7AE0FD12D22B5489E9C51ACAC2D6FDC04E8D9DBDE701B43E69E7C66477365E7CABA9BABB");
    BigNumber2048 h = FNT::multiply(A, A);
    BigNumber2048 n("eeabfbeef45ea5a122be4ea1b5b362bd3f2a3cb2963ecd606ac37be8d6fcc3ae693cd857016e638b9cfb7b386c96739bbaf2dfc1e41eb89cb62df87f16d585965276233f9ed862703ccf0126c51ba9e492644519159d174385f839ec87ff09ee5594e71744f737cffdd6bce67651c5361973c47281cd5b13c64bc285f0113da9e7952a556c0b6ee1fd99b7a04f1fc59b67d42530a9227ce9ab65fddb9f55ea672dbd53be9554ff2f8926e8ca15c3c6c9dc480893a916b5b52a3ca734a6ee0f59ba51ff68cbe2776bf9a473cc360954866fd024ef836f06038438ecf3798e05fcd438e4bb1b3b043258799e042406d61ffeb76ebd44b8bba3b058f9d44772f34d");
    BigNumber2048 r;
    BarrettReductionSpeicail(h, n, r);
    r.print();

}




void test_mul(){
    BigNumber1024 a("BB9F27A5DB4B4B9D260DCB04B56F6A7BE28602A982AC4A6994D272FB6D5C5E282BFE3704F123F74609C344918D9E7945AA832C6E03C80871B07CE46D638D34D7A0AD5B477B7A00B09DD712E57075E94D3006643C9AC14DAC3EDB355B7AE0FD12D22B5489E9C51ACAC2D6FDC04E8D9DBDE701B43E69E7C66477365E7CABA9BABB"); 
    a.print();

    BigNumber1024 b("C40194765274816CFC39466C0442D07CAE28D8BF97A80424255340EACEB504809B8D5779B909A2D5F3354BE7501D2379B845FF386A1791F230DD83E6002FFE8C61F5067E44DDCB3154E8932737E45B0F2BB02FBE4083FAB6FA027EC60F24C5CC0227F4128E89210CC501407790C2E77626C3E6A9A449C706320424DF9C0E15ED");
    b.print();


    // BigNumber2048 c = a.multiply(a, b);
    // c.print();
    // 8F A7 02 C8 DE 42 08 16638AD8D0BED38A5BF339D206F0AEE319E0A5A7BE23D2FB2B9C02554FF38AAE56B17A7285019C57D1ECA3D2D2CAC77D66DE842A45E72496FD1D0D17537723269D30E3C72F15C6EC9D9FA0EA3A4D2451F73A43ECD9BCFB578DE2C5BB6E6DBD08E8BC1236DBA24FA4450EBDAFB1754ABDBF5578BB3700A66A57BFA296061A7C2DFC4CAD9D7FFFAAA7E75E4B85731175A135B0A12AF4D5142452C1E1C3679D33FC65C96C5F488780333FCEA6DDA1051F62749A05739BE6FF9FE448C9887236F6734C00AACAE7FB2B855C8C1F44F3007392BA609B5DA69F94B2C7E22A6E65F4D4BF36E0CD0846BF07B61B237688757390F895CE8A797702AD361F
}

// 测试了一个4095长度的和4096长度的数，通过
void test_Barret_1(){
    BigNumber4096 n_2 = BigNumber4096("de843bfad788e4a5c3e8c4e8b9a6fe15720176f83c41e60342354ec66e7c33d0090bdc12bbf579b5e001b7d14bd37011c7dd355e2dc5501049ca52fa8f117d1256aa1c7219ddee88fe7ec34fcd01d63a36e876e5d45d52151c79c40ad0241ce2b835409d478e05026e3bdbbdc68378484d58b3fa429d193d89844c9d62fe48ca27904b629923e43d56fa569e09d0677acf1b9bed9163936ac84fcabb4fa934e7f4555e5beba260cdf26b952e953cdfda5d043ed63866ba219fa4b4ffa26d4907382403b48e5cb4bfdf895b0238406efadeab6f2cf53361bd3a12b1928b22603b93a14baaeed2b57fe735ccacb393a7a8e27e1a7991639f69f6577d97e9f71f3cc9788e996cea71fd12a4832cb4e096ff01a4d8c26149838f65b037f8a4cee7d78b90e1d7652acfd6fa8b99065b062013dc6b6f14e361aa3b85572aaf001541c4b5d989e94426e6d0379db03a4e959ba410681cb6dbc3d3e346eedb46b4b8f9a72be827c0413d989beff89b8462b21bba4c7733d492c5a461711658af93900f05ad096f01b6dab3e496c4392931898a8e2cf5eb23fef0c7e9c6bd4be6f6c08d2368d5250ee6510d0a10645153b7f77d8cbac5a54fc0f2b160d626d646f8eda0295a2e878525b311eb310cc7de83092d2c1f855e757033e6dac414f88b6cf15161950ebdf6c18a4932526ad0490c168b8edbc7ed562a8222a32c8fb81a4dcf4529");

    // 随机数
    BigNumber4096 h = BigNumber4096("00D773EBD80113B2387B840AF2C3A2D962F274662526C0FA35D1369FEC5FC73F3440F72BCDF19D9C04AEA0896CA0251497FF0CBE0AC52900CABD468051935D81EF5D1C33F6F0D06E196BC239B127B9095900118746FAB7D73CD34909938E9728D6D8B25CAB31C0269B4A36E60A8F51CDCC5383573CD55C6C1E220C4028B779819281331EE5D093E169F8ADC5130479817E20776CAA602931A143E501BD0B50FCB1D367D4493716FC39A1394DA82AF36685214D38708E905348BDAF1EA0EE754FC36D03C3B82BA6EA8CCE513E93DEBBF48DEEDEFA2239F894C64F581DF2A84D6E5BE3D002197874811F5065E770B8CECB5D15F16BB2EAF015F93F1443C632BC0BB3DDA70CA93132BF65392D7204545AF0E46C098B6C7D067304F7A581887F05E98F1786FE4F74394D686B5DC03A86414A91449B3DB2F24DE5EA71E08248E3F6E54E8A49AA54AD92C95CED643DEFA0FDCD79971EFB18A63B759EA6D8FB97C51AF3562EBCD8B407B2FFAE818DE393E596A1117FE014570FABD3DBABEEF7B3BCFF165A83F08573BF71F51748948643F8FD0BC4635DC5F521FC60A1D7EF76856EEB0CCF4B4A1BE8E026CD925F8046AE976C680EF82AADF82577D68AEB4C37C277BD86CBF1A8986E9CA4AAAC3F6E5C2BBC8886CF706C2DD778F0AE215B25E091AFC6BF1B7FF875CB0FF25913CFB8546090B534FAF04DF2D1ACB5F585B624FB9F10DB95");

    BigNumber4096 r("1");

    BarrettReductionFastExp1(h, n_2, r);
    r.print();
}

// 验证了四个随机数的结果，通过
void test_Barret_2(){
    BigNumber4096 n_2 = BigNumber4096("de843bfad788e4a5c3e8c4e8b9a6fe15720176f83c41e60342354ec66e7c33d0090bdc12bbf579b5e001b7d14bd37011c7dd355e2dc5501049ca52fa8f117d1256aa1c7219ddee88fe7ec34fcd01d63a36e876e5d45d52151c79c40ad0241ce2b835409d478e05026e3bdbbdc68378484d58b3fa429d193d89844c9d62fe48ca27904b629923e43d56fa569e09d0677acf1b9bed9163936ac84fcabb4fa934e7f4555e5beba260cdf26b952e953cdfda5d043ed63866ba219fa4b4ffa26d4907382403b48e5cb4bfdf895b0238406efadeab6f2cf53361bd3a12b1928b22603b93a14baaeed2b57fe735ccacb393a7a8e27e1a7991639f69f6577d97e9f71f3cc9788e996cea71fd12a4832cb4e096ff01a4d8c26149838f65b037f8a4cee7d78b90e1d7652acfd6fa8b99065b062013dc6b6f14e361aa3b85572aaf001541c4b5d989e94426e6d0379db03a4e959ba410681cb6dbc3d3e346eedb46b4b8f9a72be827c0413d989beff89b8462b21bba4c7733d492c5a461711658af93900f05ad096f01b6dab3e496c4392931898a8e2cf5eb23fef0c7e9c6bd4be6f6c08d2368d5250ee6510d0a10645153b7f77d8cbac5a54fc0f2b160d626d646f8eda0295a2e878525b311eb310cc7de83092d2c1f855e757033e6dac414f88b6cf15161950ebdf6c18a4932526ad0490c168b8edbc7ed562a8222a32c8fb81a4dcf4529");

    // 随机数
    // BigNumber8192 x = BigNumber8192("037FB536FCA0CC48E639EA55536B21E92951091B6FE155D74D03ABEC672D16B2F7397FE8149F6726718690C2FF559FD3CAC7515CD1AEC185346283822A85E9143E3871A24C5980DB8D47E6B5388B796222D33671C47E8A28070CA3497AD46908CBD5A689E61BCA2D759AB285B65EC501FD49C98742E9DC9D6EA5C1F5D717C68511F1FA2EF9D43E56E69420D39DBFC74C85FBC5791BFD97469D78B632300522BBDCDEDD641A00CBBE13725FAC0E55F93D87ABB2A17C853692691FBDC1852332CB33DC571E454495A95EABDF98CCC11037261ABE37CE2AF595B4363A32BA9CD18683537F61511427862B877D3C79AB5644177F5E2BB48DBA2183D9B772A1B85473449EEEE408C526436A166A098ECD0FD4700B4D41848123392C55F1118263E1187BA58BC80519C5B331A112660DE34455ED27EB70C60FC9755730BCD80E0F8D359B489167425B916F906818E46B14915C278E76E34907BB60899D31FEE8A5A6E5F525F8000D2C01D20D43FF4576A09F5EC8F911EA9B6AEFF20F47AA216CEDDFD1235D49A93D1E116B0CC771977D586A0FEF729BA64FB9622C44B3B8722EB9371B7C9E6C7A134F6836A00897CD2099751BCCE6F2C9C9A6B3D5354F41EEE5A1F4B8A92A2E9DB207E894AE0DF3068025E54FA0DA92582C9D0AA6CDF0861C47161FC3DD91B53C98FED9F1974A9ECF90636AF5E72C3FFD19628F1ACAFEB74594905CE386CE3F7B3618F5595103FC45C36393E4F26B4129528E891572A494E8F2EAE14152F97EBAE8DF65CB941ACC96A89DCF1210B5D1878F091FF6BB25FA3A2FC640370072644C4EB9F53C99865ED55A836C58E0813CD7F8F4E2FC7FC7999F6128383A513DDD44923A8C933D6294ADA53F2A390B2D06BFF051449143DA93B795C8AE6F361CB17AC1B9D0D5660C89A4CE208522742276E52FA3B241BE12A084F0F7289BB162A9500549D7F43BC1F59146E2877694DB65CFF4B89D291A2B03B866BEF3CF9D8A0979F17ABE127D0285E09145575FBDF0321901CEC20AF01D47513A47886C202E946A0493D6B6776D0A99E794EE63F0E91A97FE72479492DD71D1C2EAF65BAB3CD718C69EBAA2D5B491B5B0381E0AEB54878DB182ABEFADAD63D1D96A89BF9AAB38E6B425F85D4C420B17F116E9392DE1D2D842A0831E34C9343AB99CEF6BD9634BA601B7E749C824541D6277F63AD7C3132675486811B20CB7232CA411111EC36407C76E49ADFE4116E7903D4621D84F2D3532C2F5FF4F5360E77B8D7113431965F5D6FD37B12E037401B6E69C58FEE4A214D9A6C0A6F4481A05C5F22416B6276CB3F26B21534C0865AA7119BFD055FDBAC162076071CBEBFFA69AE36EC329234F7536AE247C4B9F3F49BBD6A81515E470FF129BC0EA4E65B069B0452F1FC41D2146C1AB5442F762C9F188EF9F153330EE008BD7522386C57181CE38D67E"); // 通过

    // 随机数
    // BigNumber8192 x = BigNumber8192("48032D3DA8C335ADB236F76DB2190200BC21380B30D6F2DCBB4A62EF143831F25F5CD8D69A07632E2774B43BA91911CB1B4F3045F6748B1DA22F6A0D343CA22ADBA899D2081D0D48D74E76868076F2698D5101ADF88341E906767B3F89B089B46AE7F7C46BDF6F04BCA7F102F8C65A8357F1C7699C8A548FC58690CA2AD586EFFBB3694B08476D0AEFCA2AC967F5AAAB3556495205EE4457BD5A33A3086352B4E591BB8A69C40C6F04A1079621FFA361C4D984A3F5CA5F10C19B2B619AB7AF78BBC9D6010E02C4B0919342AC5F73E28AD94AB728EF92DEB34BF4E0AF68E2C14438B9C2A6CAB3D7388255E8CCF6A893A28E81FFD195C4ACD15E8F45B8C94074A6AAC35A21FE0A40F5A60F553BF9126597A123E95831756C8414745F5E0A28CDA74BAE5FF873BA0FF518CDAF1900B72B87E04FA66C1B45CA2937531A7C9EC45311C57E24ACD9A8FD350C726C10C40E860396C64FB60885C6520DDC49C963B61B8C974DF4576934C0741A09CE4111B4A8E5FF7F24823C64E3F264FD1086FD9CD2E94206697AC64A8027E7543572C4789C5D8238B8CC63AE050269C34CB6CAB5EA53A3C0B8A2F783055CFF616CC6851D9E3E14A3191589C8C27808BBF738B4F072F385A2684CC4AA128CF21BB938197937191B11C77F2E6BA08C8961CA467C98CA212FEFAC69AB2136F4D0E3B64B70A32075D3CC99D7769C0AB8B3D13B7A0F14388BB2E87EE34364428C5EAD23DCD7A9D21B33321211AA8A3D060EBCAECB280FD5E618517A06C227F01A0DEC7EBAA1DCA48C33748672B4AD69778675ECAA64BFD0DA90A0C475FF2DD37FAA8FBF75588128EC780AB638C44FA815D5966C7DAD4FFA82EAED1348315166B6A708EA556409A107F8E8A8C977E39F24A526A0C72A4778CBE581950B8F6D91E7E7B4FB48D3726DDF3399FF1FA4FE393646432C41E99D6356ABCDFA753885205C99A5A688088D71A19920470105CAE06723A39CFC46517492FBE9EB39B6466618B45213014A0CFBE574E734C43BB4C1050FBE6ED093C904BCA6690BFB838E3A94706AC52E2F0952A20A112C8A949726AD1335F4364915DA6F9B995A5518461FFE85C746DAE38CA2D048337F4BD9D59C2C11297C655F2392890C0A2349A9B896210E01520B74602F76426017EFE16CB2EF70DE3B660A0F61B911C5ACC2BF8409CC0CBEBE1D6AD5D5698A89E6A6ED94000E4C37554C3B49F0FB915EA96D0257103F37C20C4B6CCC384C20E35F8476B099D1707D3C20F5F21DCC8FBB5151906AD6931751FB0737A010D8516FF6874D225FDED992861F60F73A38C52F34732D683A089A5BA2A8DE85C5EE8E598DA281987CA603C57E198B7335099DFB65B2739BD2B3B1D5578D16C38D147841C957261E10AD9F1C96B7A486AC22D8F4CF0B2DBC0225E00361C13D74EC57A752CDD1B2B4A118A7AAA62042A8945E"); // 通过

    // BigNumber8192 x = BigNumber8192("F430234F788835A06D290AF404CD9F022E0DBC3100FB96459E6992AC963AE0C6E72E496A0CC6FB0ECA6E9E4154D1473D1D4329A836865E449C796859DB2FD5BF0B55DBBC267478A024A0D6DA38D2C286AA19AF376544EB6B264711441349B77598AE38819BE9EDF7484FD032A7C7D619CA34BDF6A13E291FDC0F2822ADA621BC6E94A1F0B2BDCF4FB3433D42238987E7A98F6342B058D215FDDE63E36CEF4179A2207D2B3F1C8B07BCD38F382A5BF4394EA9C586C26A5E2A84AF203790A037221B88DC4970B45127A9C5AEFF35C8A703569B95C57E6F8B56E2242B67066C4FF02A6D8E34A655EFDEEC10D1A4CBD17BE97944B5A92ECB52AA676D7689BB31D36838CCB5691A42C35904E851CBEC4422F2D4CB36778C4FF65A00070DAAC0B3AEE366A8A18BBD75273C70D0BDFD983A2465197CE921017D53855D6D4C414162E17EFBDB38A86E13709C16ADC8D3C383EF5BCD9A626D2DCE7E4F29977BA2236AA047C5F6D25FEBD516242210A18F03E8BF766A97EC179B21D806C6502BF2E75FFC12819C8063139DD6FBE8F3430D6ABA0F2D2342C4B06C93ADE827CA6B4DEE8A293D88A37774554BB35BD5201A1BA6025311E09AC24D28542DE01D98F180CE99E14396DC200AC91285979FE7DD2CFCDC0CF0F3374338ADD00983A0EA5FA615135F27988D1F233766C50C633E7F6A0865CFE2527D9F010E3D3EFDB464A1B9E1705C82C1C0BBC7D2BA7AA19517249DDD82F7EA98B113547C9B28B3668F56129A76475949D6004285EB4804CEAA4041905CC54923CED7673DC7A25F4B70773A0CED094DD6B7BC64A7C45A6CD89A2E938796B1B03E51D69C2FB59575745444C1D2763ACADEDA09DC1E5FBCBC297260E6C305465A6C641A61A8290FA6D3A4456C73147C51E568C764475CB54B676CB8420FDDC1AA708140A18126660250CC287330F7A377B3D711B98E8057D77A889EFAEE4D9C13D42E2E8EA31DF95D8976015127C6BD1723174E80ACDC1997EF960234005414E77A8AA1808AB7B27C95B9038A9E8BA85B313A35219673DB35BE609EB5F2384EBF74936946A58A2389EFD24C71E6C208F582AE9C76CB744880D1FA95BA09078677D54CD4FF9E49C27E1FE9780D0AF38343F349F77DF7021B80EB677C849340A47B95AB5324C10047701423B24B06FCAFC248132E65F74D4EAB38F72567C19BD69EE2B793E48C5EF158106C4B4B05FCF5B384CB4938FEAE6148F5A43EE06C3BAA33EC962606B0BA4D716B850DA0215344030AD639DC123C8329D6F199D36191DB6F09D1D023A90C676B0A0BE6AE286A50A2284AD9F50A9156E2748F90F610D1D11073A7C60E899A87318E6626C62F7AB265D859AD55FC3D28EA63B554737A8F8E8E1AD2BFCC34E04977EFCAAA448E443183AD681BA79058B74C8FDB13D693DAFD1507A20304775B217F157B1E94EF307FC0"); // 通过

    BigNumber8192 x = BigNumber8192("950A5A0EE9E27170028AF5749D829D014BB7A3DC79E1E098F5356E058AEB2259380C6196B819FF3D60A71C20C9CE36A338184C404E45C5184F1D399775652294FEFDA44D29D5A5D3914935679A29491810B8A4C4D861141E5A5518615745D65607B38D5D11AD42C13524903601648CDA70E846A4572C1F0644290783B1399221423111921CA8AF824A7FFF455ACD3B837414B170389E5115F15F51CFD3CBF614D74877EBDB62A6578907CB6946CD1C300B98AF5097A74F53A883F3AB9201D2B7F7BCC43E3F7E5D205DC0C021F766E63256F1274ECE8042CFBF066E8B34C5C2931A4A161D6EAF285721DA29D1386D9A97BCB04FEB1AAFB0A6AB45D4F54EFEDD152A74A1FAD83F158F11BB29C55E5FC1050B3DCAC84AE630EE9ED265524A01EC34A5FFD693DEBBB5535DE13299EB5BEC7DF9638D41C80C9EA2EA8B6F3AE370B2FF00250D798D131500DB34D21DE7474CF82F792AAD7AFE07CDB25D28936CA25E9533E4FC39FB7E1BB232F5D642171CACEC5EBF6A33154B94D20D691656FAE34A89E8227BB1228D5554610090448C3D7DE9F3738C162C10B5E90E604FAC1053BB46423D7B7B49831341C044A6CC8B857AD6C51521E21A6B5080FAA20FCD9C15F41BC14E515F777FBC113E021CD53B6EE6EB69421618DD9C6AA1E16660D165B5DFA15CC63E459F4D3309164AB51B78364B518159A4FDA4DC026BB9C7CDB2B52656D7DBB0F61BEB9CA43676D1D2A62690B5678E2F3403451FBEECC08462F1CF3B12ACBB1AA4CCB6C3CC49AF4D959F1AFE49E3E4CA875F8D1DA5F31A4923DE27CF96F34A2F5C73940903161079F21494827F2EDF1074893615A89DF5EBF71B0D43FB7A1D127A4ED843366FBE0B2AFB9DECDF5DB3573092C9A5EA12735799BE1ED7146885F7C6596BC33BBB990A99A9714E570AAA22B3A09B9BC3C623AFCD76AF196AF141D6B6A9E3ECD97545D241FCB9BD022CBFBD9CDE9E105D1730A6CEF61A25C4807647B779A24FEAEA4B689BC1F3CB95192C0510CE8BBC9C1373288FCFAC6F467B1E6A6942BBE1862B7388260C1BCBA4C04A1B6707109ACCE0F3238D8841A15918935346D60EC2921F39629ED7A8246D5F7C240076AEF051629B30CEA363D8660A04D936E80D5A774C38A0CFA698F6A5A5CECA6BE50DE2568075ACEF9CE2AA324E4F2B4E9A6517D099820834F5259D4DBA5AB6A280E9D3CB6CCA9C10DA5187F29FF42D9FD9108B2143B627D7FA0F1051F15B71C26C01C852F0B61922C05A67F3E914F0DDD4A2605185941B9796CBEEA18891258255434B098F731FC11B670A980C14270C7550220322F09486065BA3F688BD44B23B447EF00CC27FB567DE96C28E87CF0242E5F1FB0CA4E69F28875FBFDCFCEC6739827B3A80D12B7A0C3D7EDD26F058E3E99A6C93B4F1F27C414E8F3A0DDE6EDA3686259E21E482B2695DF41D96"); // 通过

    BigNumber4096 r("1");
    BarrettReductionFastExp2(x, n_2, r, N2_ARRAY);
    r.print();
}

// 验证了一些随机数，通过
void test_fast_exp(){
    BigNumber4096 n_2 = BigNumber4096("de843bfad788e4a5c3e8c4e8b9a6fe15720176f83c41e60342354ec66e7c33d0090bdc12bbf579b5e001b7d14bd37011c7dd355e2dc5501049ca52fa8f117d1256aa1c7219ddee88fe7ec34fcd01d63a36e876e5d45d52151c79c40ad0241ce2b835409d478e05026e3bdbbdc68378484d58b3fa429d193d89844c9d62fe48ca27904b629923e43d56fa569e09d0677acf1b9bed9163936ac84fcabb4fa934e7f4555e5beba260cdf26b952e953cdfda5d043ed63866ba219fa4b4ffa26d4907382403b48e5cb4bfdf895b0238406efadeab6f2cf53361bd3a12b1928b22603b93a14baaeed2b57fe735ccacb393a7a8e27e1a7991639f69f6577d97e9f71f3cc9788e996cea71fd12a4832cb4e096ff01a4d8c26149838f65b037f8a4cee7d78b90e1d7652acfd6fa8b99065b062013dc6b6f14e361aa3b85572aaf001541c4b5d989e94426e6d0379db03a4e959ba410681cb6dbc3d3e346eedb46b4b8f9a72be827c0413d989beff89b8462b21bba4c7733d492c5a461711658af93900f05ad096f01b6dab3e496c4392931898a8e2cf5eb23fef0c7e9c6bd4be6f6c08d2368d5250ee6510d0a10645153b7f77d8cbac5a54fc0f2b160d626d646f8eda0295a2e878525b311eb310cc7de83092d2c1f855e757033e6dac414f88b6cf15161950ebdf6c18a4932526ad0490c168b8edbc7ed562a8222a32c8fb81a4dcf4529");

    // 随机数：底数x
    // BigNumber2048 x = BigNumber2048("988E4E692C953098C7ECFC65A5231A72F3C382D7DC8119DED6AC8CD3789A3FD1354FC66E2AC41E1DE5E07C176178D6030A94EF970E0BD07FA2A023523154F71FC35BB9767432863267F3790F620AD2773C62CA8284DAC725C13EC35A6992B861645D38EB9AFF8DE1CB165F36385245738CCCD915C03C7988C613D8C6D1B15D378171378D6990F6C6364570E94A4C018629964F733305B9190FD9338108A6EA1717D7B1287786D4DB063A805C31AF1EE78872A6C621631E9EDB297D36AED6811459934A63501F9E468AA4C2499053F8EB58FCF4AA949B98DA9DE88A9EA16E6580C7593C38E4EFC7D4A2FB899A69D51E3224E9D1FD337B5FA56A39C9A739F11712"); // 通过

    BigNumber2048 x = BigNumber2048("3A44177BC454E1A9AEAA881398502D2F8BE88441EFA3C60C0FAF6E98CC34EA0C0BCA795FAF37E64DC48028B069E3684663394E563D79B9991BE00B812ABA0172C1BCD9F139F62E9C3FE3FF08A896FF1170180B559A8607B8D7F060E19A72E642BDE73FDC5374EEF61BD669C3F88063E8D1C331BEBB076E9D55DEB7E653BD77B77A25753C5C8D9A135F0BB4D173365779AF30635C8B54822FEA6A0A7D1ED6695ECFE6CF5144B7ACC82EF7F23294535E80DCA3402D6D6C36549907145BF02281AD0C39AEC29C41BA11512E7B01057E95D2EC175E4183B23F7B52FE63EE13770BCC36292D36B63D6F41F9640A92F6B8770C3BB43C9A5E162CB0A89BF08DD57B8251"); // 通过

    // 指数n
    // BigNumber2048 n = BigNumber2048("1001"); // 通过

    // 指数n 随机数
    BigNumber2048 n = BigNumber2048("0x3E24DE5216C4BF39E3FDFF1502EB9EABA7EB52F6CC8B4D017A623B42B2446C236313CC07473A72CFA129B2F7A2557B09C6C72E6761384801D4463730883D7842DEF76A55B5646B56CF147558E498EB9D2512A8324B5586E271CF261096D42CF053814DFAEBA1CD7789A74E31047AC2E4CBABE94359D3DEF62F7B5FDFA57D33FD5A73D81F0167D54143054F4AF0CD15BA1D4EC06F7A4059609BC387F1652F00BD4E48ED03EAC4D9F98DD7FAEB5300843AAD0EAF14F4802DB02058B429CC79D582BA6DE5032A3B74E8655AA364D562F8A24ED62002FB319D15E7E94EDB4329D1BE71FA98C8C9BF83A939D0D23C574767015E5F69A47CA1CCE692FBDC37D4D1E094"); // 通过

    // 结果r
    BigNumber4096 r("1");
    FastExp(x, n, n_2, r);
    r.print();

}

// 通过
void testModp2(){
    BigNumber1024 p = BigNumber1024("fc2ffe76bf35218c3cf386fd77d3fd46d760db0e5135dfa0c59932e9b4fbb0f3918363690616084105e6d7994d29916ddc1b841366cef4bc9d90bc9075d86e577bb6a34bbb7aec8813d5b78e124b8e452d811b2ed461c741055e59ad1c27f4d08f55ab1197604a01589fb1e094b4fd7d61c68fffbdd709721c7ab559abc1cc01");

    BigNumber2048 p_2 = FNT::multiply(p, p);

    // A 是随机数
    BigNumber8192 A = BigNumber8192("91782944f645cf148573941f733ed5b6bc4c994c2bea1d938f4cb9868d54e4741a496f9fa31027d57802549bbe43a9b008bdd55732edd2c3b79769ca5255fc5fa943e047195b07c2f392dddeb184a07a5eee64cd8951c998c074c7c6b8b23075f17c6c9e2cbf3a9c0c3e99290db0d7fe28d83ec00c719a81d1413801950d9d4c24b5f71633c4dc5d8fdcd6742285bace256c641a17aa2aaa3bc12e972c945778a1c56cd45951f8f5dd9677e12e7d8ae42726e5788cb17ea9bcc5bf28751b64dfb7ff61c9ed2d0bc0e8a8c9b1e6c0479e7d95cbeebe7781e5eef1eadba409afdbde2c9ef4afe900d4323b16c57c32ab74c9fe1085781706bf0c526f1e277467a78cec02ae56df65dc9bee2d685524148625e9f11b34a08bea432815a437648a03bc5550f5990f5cee2ae139372bdaab85cda765739d8a74bb20b20426dc50afc48f2bd12ef5dc6668cf13dcf90417dfc6f65ab918950b441ee594299c58a717606986ee7aa466a5aca5118ead3b90bebe52c5e3381b5e50baaeff3193d46049d0b296809132b03b3f17c6741ae12c28771aa7c4a7a60281528b8308046a80380aef91a9323c6737183dd60fc7ddc7ee08aa580318573498e992391d5f84e0978dc33dff7726ee0b88dfe812d69022c8e136671255a1c504c94c87320c7ca05c27635e1d2b3d06a5d62961b81ee642b169e0253eba216e1c88d94ac30790ccbc36");

    BigNumber2048 r;

    BarretReductionModp2(A, p_2, r, P2_ARRAY);
    r.printHexNumber("r");
}
