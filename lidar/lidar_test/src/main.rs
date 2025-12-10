use tokio::net::UdpSocket;

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    let socket = UdpSocket::bind("0.0.0.0:2368").await?; 
    println!("Listening on UDP 2368...");

    let mut buf = [0u8; 2048];

    loop {
        let (len, addr) = socket.recv_from(&mut buf).await?;
        println!("Received {} bytes from {}", len, addr);
    }
}
