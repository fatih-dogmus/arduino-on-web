const btnConnect = document.getElementById("connect");

class LineBreakTransformer {
    constructor() {
        this.container = '';
    }

    transform(chunk, controller) {
        this.container += chunk;
        const lines = this.container.split('\r\n');
        this.container = lines.pop();
        lines.forEach(line => controller.enqueue(line));
    }

    flush(controller) {
        controller.enqueue(this.container);
    }
}

async function connect() {

    const port = await navigator.serial.requestPort();
    await port.open({ baudRate: 9600 });

    let reader;
    const decoder = new TextDecoderStream();
    const transformer = new TransformStream(new LineBreakTransformer());

    while (port.readable) {

        reader = port.readable.pipeThrough(decoder).pipeThrough(transformer).getReader(); // veriler seri geldiği için satır satır gelmeyebiliyor. yeni satıra indikçe almak için dönüştürüyoruz.
        try {
            while (true) {
                const { value, done } = await reader.read(); // değerleri oku

                if (done) {
                    break; // okuma tamamlandığında çık
                }

                if(value.includes("#")){
                    document.querySelector("body").style.setProperty("--bulbColor", value);
                }else{
                    document.getElementById("watt").innerHTML = "Birim saniyede ölçülen, toplam watt " + value;
                }

                
            }
        } catch (e) {
            console.log(e);
        } finally {
            reader.releaseLock(); // tekrar okuyabilmek için
        }
    }

    await port.close();
    reader.cancel();
}


btnConnect.addEventListener("click", () => {
    if ("serial" in navigator) {
        connect();
    } else {
        console.log("tarayıcı desteklemiyor");
    }
});


navigator.serial.addEventListener("connect", async (e) => {

});

navigator.serial.addEventListener("disconnect", e => {
    console.log("bağlantı koptu");
});
