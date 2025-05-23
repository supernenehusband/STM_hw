# start.py
import queue
import threading
from ble_listener import start_ble_listener
from main import run_game

def start_game():
    ble_queue = queue.Queue()

    print("🚀 Starting BLE thread...")
    ble_thread = threading.Thread(target=start_ble_listener, args=(ble_queue,), daemon=True)
    ble_thread.start()

    print("🎮 Starting main game loop...")
    run_game(ble_queue)

if __name__ == "__main__":
    start_game()