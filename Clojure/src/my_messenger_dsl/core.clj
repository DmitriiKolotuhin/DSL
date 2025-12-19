(ns my-messenger-dsl.core
  (:gen-class))

; --- Определение DSL ---

; Атомы для хранения состояния и обработчиков
(def state (atom {}))
(def event-handlers (atom {}))

; Макрос для определения начального состояния
(defmacro defstate [name & body]
  `(reset! state ~(first body)))

; Макрос для регистрации обработчика события
; Теперь обработчик должен принимать [current-state event-data]
(defmacro on-event [event-type [state-sym event-data-sym] & body]
  `(swap! event-handlers assoc ~event-type
          (fn [~state-sym ~event-data-sym] ~@body)))

; Функция для вызова события
(defn trigger-event [event-type & {:keys [event-data]}]
  (if-let [handler (get @event-handlers event-type)]
    (do
      (println "Processing event:" event-type "with " event-data)
      ; Используем анонимную функцию для вызова handler с правильными аргументами
      (swap! state (fn [current-state] (handler current-state event-data)))
      (println "New state:" @state))
    (println "No handler found for event:" event-type)))

; --- Функция main ---
(defn -main
  "Основная функция для запуска демонстрации DSL."
  [& args]
  (println "=== Starting Messenger State DSL Demo ===")

  ; --- Определение состояния ---
  (defstate client-state
    {:chats {1 {:id 1 :name "General" :messages []}
             2 {:id 2 :name "Random" :messages []}}
     :current-chat-id 1
     :user {:id 1 :name "Alice" :status :online}})

  (println "Initial state:")
  (println @state)
  (println "-------------------")

  ; --- Определение обработчиков событий ---
  (on-event :new-message [s data]
            (let [chat-id (:current-chat-id s)
                  new-message {:id (rand-int 10000) :text (:text data) :sender (:sender data)}]
              (update-in s [:chats chat-id :messages] (fnil conj []) new-message)))

  (on-event :switch-chat [s data]
            (assoc s :current-chat-id (:chat-id data)))

  (on-event :update-user-status [s data]
            (assoc-in s [:user :status] (:status data)))

  ; --- Демонстрация работы ---
  (println "Triggering :new-message event...")
  (trigger-event :new-message :event-data {:text "Hello, world!" :sender "Bob"})

  (println "\nTriggering :switch-chat event...")
  (trigger-event :switch-chat :event-data {:chat-id 2})

  (println "\nTriggering :new-message event in new chat...")
  (trigger-event :new-message :event-data {:text "Hi from Random!" :sender "Charlie"})

  (println "\nTriggering :update-user-status event...")
  (trigger-event :update-user-status :event-data {:status :away})

  (println "\nFinal state:")
  (println @state)
  (println "=== Demo Finished ==="))