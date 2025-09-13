#!/bin/bb

(ns combos
  (:refer-clojure :exclude [keys])
  (:require [clojure.string :as str]))

(def keys {:a "LGUI_T(KC_A)"
           :r "LALT_T(KC_R)"
           :s "LCTL_T(KC_S)"
           :t "LSFT_T(KC_T)"
           :n "RSFT_T(KC_N)"
           :e "RCTL_T(KC_E)"
           :i "LALT_T(KC_I)"
           :o "RGUI_T(KC_O)"})

(defn kw->key [kw]
  (if (string? kw)
    kw
    (let [default (str "KC_" (str/upper-case (name kw)))]
      (get keys kw default))))

(def combos
  {[:w :p] :tilde
   [:w :f] :exlm
   [:f :p] :scln
   [:s :p] :at

   [:x :c] :plus
   [:c :t] :minus
   [:x :d] :astr
   [:s :d] :eql

   [:t :f] :bsls

   [:w :r] :lbrc
   [:f :s] :lcbr
   [:p :t] :lprn
   [:b :g] :lt

   [:l :y] :hash
   [:l :u] :coln
   [:u :y] :dlr
   [:e :l] :perc

   [:r :s :t] "OSM(MOD_LSFT)"

   [:n :e :i] "OSM(MOD_RSFT)"

   [:e :h] :grv
   [:h :dot] :ampr
   [:n :comm] :unds
   [:comm :dot] :circ

   [:n :u] :pipe

   [:j :m] :gt
   [:l :n] :rprn
   [:u :e] :rcbr
   [:y :i] :rbrc

   [:a :z] :1
   [:r :x] :2
   [:s :c] :3
   [:t :d] :4
   [:g :v] :5
   [:m :k] :6
   [:n :h] :7
   [:e :comm] :8
   [:i :dot] :9
   [:o :slsh] :0})

(defn combos->header-lines [combos]
  (let [with-names (map-indexed (fn [idx combo]
                                  (conj combo (str "combo_" idx)))
                                combos)]
    (-> []
        (into (for [[keys _ name] with-names
                    :let [keystr (->> keys
                                      (map kw->key)
                                      (str/join ", "))]]
                (format "const uint16_t PROGMEM %s[] = {%s, COMBO_END};" name keystr)))
        (conj "combo_t key_combos[] = {")
        (into (for [[_ res name] with-names]
                (format "\tCOMBO(%s, %s)," name (kw->key res))))
        (conj "};"))))

(->> combos
     combos->header-lines
     (str/join "\n")
     (spit "combos.h"))
