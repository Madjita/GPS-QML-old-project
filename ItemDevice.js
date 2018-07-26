// factorial.js
 .pragma library


var mdelButton,radioButtonVKl,modelIp,gsg,n6700_1,n6700_2

//var mdelButtonlist = new Array(),
//    radioButtonVKllist = new Array(),
//    modelIplist = new Array()

var view
var count

function setView(viewGet) {
    view = viewGet;
    count = view.count;
    flag=1;
}

//var list2 = new Array();

var flag;


//var list = new Array();

//function set(mdelButtonGet,radioButtonVKlGet,modelIpGet,gsgGet,n6700_1Get,n6700_2Get)
//{
//   if(flag <count)
//   {


//        flag++;

//        mdelButtonlist.push(mdelButtonGet);
//        radioButtonVKllist.push(radioButtonVKlGet);
//        modelIplist.push(modelIpGet);
//        gsg = gsgGet
//        n6700_1 = n6700_1Get
//        n6700_2 = n6700_2Get



//       console.log(mdelButtonlist);

//   }
//   else
//   {
//       flag = 1;
////       for(var i=0; i < list2.length;i++)
////       {
////            list2.pop();
////       }
//   }
//}

function all(connect) {
    if(connect.text == "Подключить")
    {
        connect.text = qsTr("Отключить");

        closseAll();

        setGSG();
        setN6700_1();
        setN6700_2();
    }
    else
    {
        connect.text = qsTr("Подключить");
        closseAll();
    }

}

function closseAll()
{
    // @disable-check M306
    gsg.DisConnect();
    // @disable-check M306
    n6700_1.DisConnect();
    // @disable-check M306
    n6700_2.DisConnect();
    radioButtonVKl.text = qsTr("Отключен");
    radioButtonVKl.checked = false;
    mdelButton.text = qsTr("Подключить");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Используется , выше нет.

function set2(mdelButtonGet,radioButtonVKlGet,modelIpGet,gsgGet,n6700_1Get,n6700_2Get)
{
    mdelButton = mdelButtonGet;
    radioButtonVKl = radioButtonVKlGet;
    modelIp = modelIpGet;
    gsg = gsgGet
    n6700_1 = n6700_1Get
    n6700_2 = n6700_2Get
}


function setGSG2(mdelButton,modelIp,gsg,modelDevice3,menuRectItemDashboard2) { //mdelButton,radioButtonVKl,modelIp,gsg

        if(mdelButton.text == "Подключить")
        {

            gsg.slot_connectDevice(modelIp.text);


            if(gsg.slot_connectOk())
            {

                 mdelButton.text = qsTr("Отключить");

                modelDevice3.text = gsg.name;

                menuRectItemDashboard2.color = "green"

            }
            else
            {

               modelDevice3.text = gsg.name;

               menuRectItemDashboard2.color = "red"
            }
        }
        else
        {
            // @disable-check M306
            gsg.slot_DisConnect();
            mdelButton.text = qsTr("Подключить");
            modelDevice3.text = qsTr("");

            menuRectItemDashboard2.color = "#7e827f"
        }

}

 function setGSG() { //mdelButton,radioButtonVKl,modelIp,gsg

         if(mdelButton.text == "Подключить")
         {

            // gsg.slot_connectDevice(modelIp.text.split(": ")[1]);

              gsg.slot_connectDevice(modelIp.text);

             if(gsg.slot_connectOk())
             {
                  radioButtonVKl.text = qsTr("Подключен");
                  radioButtonVKl.checked = true;
                  mdelButton.text = qsTr("Отключить");

             }
             else
             {
                 // @disable-check M306
                 gsg.slot_DisConnect();
                 radioButtonVKl.text = qsTr("Отключен");
                 radioButtonVKl.checked = false;
                 mdelButton.text = qsTr("Подключить");
             }
         }
         else
         {
             // @disable-check M306
             gsg.slot_DisConnect();
             radioButtonVKl.text = qsTr("Отключен");
             radioButtonVKl.checked = false;
             mdelButton.text = qsTr("Подключить");
         }

 }



function setN6700_12(mdelButton,modelIp,gsg,modelDevice3,menuRectItemDashboard2)
{

    if(mdelButton.text == "Подключить")
    {

        gsg.slot_connectDevice(modelIp.text);

        if(gsg.slot_connectOk())
        {

             mdelButton.text = qsTr("Отключить");

            modelDevice3.text = gsg.name;

            menuRectItemDashboard2.color = "green"

        }
        else
        {

           modelDevice3.text = gsg.name;

           menuRectItemDashboard2.color = "red"
        }
    }
    else
    {
        // @disable-check M306
        gsg.slot_DisConnect();
        mdelButton.text = qsTr("Подключить");
        modelDevice3.text = qsTr("");

        menuRectItemDashboard2.color = "#7e827f"
    }

}

function setN6700_1() {

        if(mdelButton.text == "Подключить")
        {

            // @disable-check M306
           // if(n6700_1.Connect(modelIp.text.split(": ")[1]))

            n6700_1.slot_connectDevice(modelIp.text.split(": ")[1])

            if(n6700_1.slot_connectOk())
            {
                 radioButtonVKl.text = qsTr("Подключен");
                 radioButtonVKl.checked = true;
                 mdelButton.text = qsTr("Отключить");
            }
            else
            {
                // @disable-check M306
                n6700_1.DisConnect();
                radioButtonVKl.text = qsTr("Отключен");
                radioButtonVKl.checked = false;
                mdelButton.text = qsTr("Подключить");
            }
        }
        else
        {
            // @disable-check M306
           // n6700_1.DisConnect();

            n6700_1.slot_DisConnect();

            radioButtonVKl.text = qsTr("Отключен");
            radioButtonVKl.checked = false;
            mdelButton.text = qsTr("Подключить");
        }

}

function setN6700_2() {

        if(mdelButton.text == "Подключить")
        {

            // @disable-check M306
            if(n6700_2.Connect(modelIp.text.split(": ")[1]))
            {
                 radioButtonVKl.text = qsTr("Подключен");
                 radioButtonVKl.checked = true;
                 mdelButton.text = qsTr("Отключить");
            }
            else
            {
                radioButtonVKl.text = qsTr("Отключен");
                radioButtonVKl.checked = false;
                mdelButton.text = qsTr("Подключить");
            }
        }
        else
        {
            // @disable-check M306
            n6700_2.DisConnect();
            radioButtonVKl.text = qsTr("Отключен");
            radioButtonVKl.checked = false;
            mdelButton.text = qsTr("Подключить");
        }

}
