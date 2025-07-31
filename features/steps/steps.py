import os
import subprocess
from behave import given, when, then

@given('используем тестовый файл "{filename}"')
def step_impl(context, filename):
    context.current_test_file = os.path.join(context.test_data_dir, filename)

@when('ищем пару слов "{word1}" "{word2}" на расстоянии не более {max_dist} слов')
def step_impl(context, word1, word2, max_dist):
    cmd = [context.app_path, context.current_test_file, word1, word2, max_dist]
    context.process = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        encoding='cp1251'
    )
    context.stdout, context.stderr = context.process.communicate()

@when('пытаемся искать с нечисловым расстоянием "{invalid_dist}"')
def step_impl(context, invalid_dist):
    cmd = [context.app_path, context.current_test_file, "налоги", "России", invalid_dist]
    context.process = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        encoding='cp1251'
    )
    context.stdout, context.stderr = context.process.communicate()

@when('пытаемся искать с отрицательным расстоянием')
def step_impl(context):
    cmd = [context.app_path, context.current_test_file, "налоги", "России", "-5"]
    context.process = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        encoding='cp1251'
    )
    context.stdout, context.stderr = context.process.communicate()

@when('пытаемся искать в несуществующем файле')
def step_impl(context):
    cmd = [context.app_path, "nonexistent.txt", "налоги", "России", "5"]
    context.process = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        encoding='cp1251'
    )
    context.stdout, context.stderr = context.process.communicate()

@when('запускаем программу без аргументов')
def step_impl(context):
    cmd = [context.app_path]
    context.process = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        encoding='cp1251'
    )
    context.stdout, context.stderr = context.process.communicate()

@then('находим {expected_count:d} совпадение(ий)')
def step_impl(context, expected_count):
    assert str(expected_count) in context.stdout
    assert context.process.returncode == 0

@then('получаем сообщение об ошибке "{message}" с кодом {code:d}')
def step_impl(context, message, code):
    assert message in context.stderr
    assert context.process.returncode == code