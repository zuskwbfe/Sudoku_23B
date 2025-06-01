#include "SudokuCell.h"
#include <QColor>
#include <QDebug>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

SudokuCell::SudokuCell(int r, int c, QWidget *parent)
    : QPushButton(parent), row(r), col(c) {
  setFixedSize(40, 40);
  setStyleSheet(
      "font-size: 20px; border: none; margin: 0; padding: 0;"); // Базовый стиль
  connect(this, &QPushButton::clicked,
          [this]() { emit cellClicked(row, col); });
}

// Установка отображаемого значения в ячейке
void SudokuCell::setDisplayValue(int value) {
  currentValue_ = value; // Сохраняем текущее значение
  setText(value > 0 ? QString::number(value) : "");
  if (value != 0) {
    clearNotes();
  }
  updateTextColor(); // Обновляем цвет текста
}

void SudokuCell::setOriginal(bool isOriginal) {
  isOriginal_ = isOriginal;
  updateTextColor(); // Обновляем цвет текста при изменении флага
  update(); // Триггер перерисовки
}

// Обновление цвета текста в зависимости от типа ячейки
void SudokuCell::updateTextColor() {
  QPalette p = palette();
  if (currentValue_ > 0) {
    p.setColor(QPalette::ButtonText,
               isOriginal_ ? Qt::black : QColor(75, 75, 155)); // цифра
  } else {
    p.setColor(QPalette::ButtonText, Qt::black);
  }
  setPalette(p);
}

// Устанавливаем состояние подсветки и перерисовываем ячейку
void SudokuCell::setHighlightState(HighlightState state) {
  if (highlightState != state) {
    highlightState = state;
    update();
  }
}

// Переопределяем отрисовку для поддержки подсветки
void SudokuCell::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Рисуем фон в зависимости от состояния подсветки
  switch (highlightState) {
  case Selected:
    painter.fillRect(rect(), QColor(120, 120, 190)); // Фиолетовый
    break;
  case Related:
    painter.fillRect(rect(), QColor(187, 187, 222)); // Сереневый
    break;
  case SameDigit:
    painter.fillRect(rect(),
                     QColor(120, 120, 190)); // Фиолетовый
    break;
  default:
    painter.fillRect(rect(), Qt::white);
  }

  // Рисуем текст
  if (!text().isEmpty()) {
    painter.setPen(palette().color(QPalette::ButtonText));
    painter.setFont(QFont("Arial", 16));
    painter.drawText(rect(), Qt::AlignCenter, text());
  } else {
    painter.setFont(QFont("Arial", 8));
    int cellWidth = width() / 3;
    int cellHeight = height() / 3;

    for (int noteIndex = 0; noteIndex < 9; ++noteIndex) {
      if (notes_[noteIndex]) {
        int noteRow = noteIndex / 3;
        int noteCol = noteIndex % 3;
        QRect noteRect(noteCol * cellWidth, noteRow * cellHeight, cellWidth,
                       cellHeight);
        painter.drawText(noteRect, Qt::AlignCenter,
                         QString::number(noteIndex + 1));
      }
    }
  }

  // Рисуем границы
  painter.setPen(QPen(Qt::lightGray, 1));

  // Всегда рисуем правую и нижнюю границы
  painter.drawLine(rect().topRight(), rect().bottomRight());
  painter.drawLine(rect().bottomLeft(), rect().bottomRight());

  // Утолщенные границы для разделения блоков 3x3
  if (col == 2 || col == 5) { // Правая граница для столбцов 2 и 5
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(rect().topRight(), rect().bottomRight());
  }

  if (row == 2 || row == 5) { // Нижняя граница для строк 2 и 5
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(rect().bottomLeft(), rect().bottomRight());
  }

  // Убираем границы для крайних правых и нижних ячеек
  if (col == 8) { // Последний столбец - не рисуем правую границу
    painter.setPen(Qt::NoPen);
    painter.drawLine(rect().topRight(), rect().bottomRight());
  }

  if (row == 8) { // Последняя строка - не рисуем нижнюю границу
    painter.setPen(Qt::NoPen);
    painter.drawLine(rect().bottomLeft(), rect().bottomRight());
  }
}

void SudokuCell::mouseDoubleClickEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    emit cellDoubleClicked(row, col); // Испускаем сигнал двойного клика
    event->accept(); // Помечаем событие как обработанное
  } else {
    QPushButton::mouseDoubleClickEvent(event); // Для других кнопок мыши
  }
}

void SudokuCell::setNote(int value, bool on) {
  if (value >= 1 && value <= 9) {
    notes_[value - 1] = on;
    update();
  }
}

bool SudokuCell::hasNote(int value) const {
  return (value >= 1 && value <= 9) ? notes_[value - 1] : false;
}

void SudokuCell::clearNotes() {
  notes_.fill(false);
  update();
}
