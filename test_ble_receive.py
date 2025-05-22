import asyncio
from bleak import BleakClient, BleakScanner

TARGET_NAME = "BlueNRG"
CHAR_UUID = "00000100-0000-0001-11e1-ac360002a5d5"


async def find_target_device():
    print("🔍 Looking for BlueNRG...")
    while True:
        devices = await BleakScanner.discover(timeout=3)
        for d in devices:
            if d.name == TARGET_NAME:
                print(f"✅ Found {TARGET_NAME}: {d.address}")
                return d.address
        print("⏳ Not found yet, retrying...")


async def listen_for_data(address):
    async with BleakClient(address) as client:
        print("🔗 Connecting...")
        await client.connect()
        print("✅ Connected!")

        def handle_notification(_, data):
            try:
                text = data.decode("utf-8").strip()
                print(f"[RECV] {text}")
            except Exception as e:
                print(f"[ERROR] Invalid data: {e}")

        print("📡 Subscribing to notifications...")
        await client.start_notify(CHAR_UUID, handle_notification)

        print("📥 Listening for messages (Ctrl+C to stop)...")
        try:
            while True:
                await asyncio.sleep(1)
        except KeyboardInterrupt:
            print("🛑 Stopping...")
        finally:
            await client.stop_notify(CHAR_UUID)

if __name__ == "__main__":
    asyncio.run(listen_for_data(asyncio.run(find_target_device())))
